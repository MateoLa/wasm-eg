<div align="center">

<img src="assets/wasm4.svg" width="150" height="150"/>

<h3>Running C/C++ in the Browser</h3>

<p>WebAssembly examples</p>

</div>


#### WebAssembly 

Wasm is a binary instruction format for a stack-based virtual machine. The code can be run in modern browsers to run compiled codes like C++, Rust, Go, etc. on the client.

WebAssembly is designed to complement and run alongside JavaScript, sharing functionality between them.


#### Prerequisites

Install GCC/g++ compilers required to compile C++ programs in Linux

```sh
sudo apt install build-essential
```

Install or Update Emscripten

```sh
git clone https://github.com/emscripten-core/emsdk.git  --> download Emscripten if you haven't already

cd emsdk
./emsdk update  --> update Emscripten is you already have it installed
./emsdk install latest
./emsdk activate latest  --> configuration files
source ./emsdk_env.sh  --> set Emscripten into your current console

emcc -v
emcc --version
```


#### Hello World Example

In a browser we going to run the main() C++ function in hello.cpp.

```sh
cd hello
emcc hello.cpp -o hello.html --emrun
emrun hello.html
```

emcc, the Emscripten compiler, compiles hello.cpp into a binary `hello.wasm` file which is instantiated by `hello.js` glue code.

emrun is a local web sever and test tool used to host and launch the compliled html and WebAssembly files.

And that's it. We are using hello.js glue code to run C code into the browser. <br>
Emrun capture application stdout and stderr streams during the run and print them to a terminal. We going to see "Greetings from MaLa" in the emrun server console and in the browser's console and then the program ends.


#### Calling C/C++ functions from Javascript

<div align="center">

<img src="assets/ping-pong.png" width="150" height="150"/>

<h4>Ping Pong example</h4>

</div>

From a Ping-Pong game entirely in JS (/original/pong.js), we will migrate some functions to pong.cpp and use them from there. <br>
We choose the function `getAIMove()` to be moved to pong.cpp. 


we will use a tool called embind (part of the Emscripten toolchain) to bind the C++ function to JS: --bind option. 

```sh
cd pong/start
emcc pong.cpp -o pp_wasm.js --std=c++17 --bind
emrun pong.html --no_emrun_detect
```

The directory structure is as follows:
  - /original, the ping-pong game in its original JS and html files.
  - /start, the initial modifications to make the wasm module work
  - /final, the final WebAssembly ping-pong game


`pong.js` instantiates a variable `Module` which enables the use of the functions and classes defined in C/C++.
You could read the comments at the begining of any JS glue code to learn how to use the variable.<br>
Here we use the 4th option. In `pong.html` we define a Module variable to execute render().<br>
Then the "pong.js" adds to it all the C++ functions and classes we going to use.

Note also that pong.html use Emscripten event `onRuntimeInitialized` to delay function calls until the Wasm artifact is fully loaded.

We modify pong.js to call the binded function by the glue code. `Module.getAIMove()` is the C++ function we moved to pong.cpp.

/final shows how to pass complex data with Embind. <br>
Embind supports binding classes, pointers, arrays, smart pointers, memory views, inheritance and polymorphism.<br>
Also you can write JS functions inside C/C++. This type of js block must be declared using `EM_JS` emscripten tool and can be called from the C++ code.

And thus, we have owr ping-pong game running in a browser <br>

Here we are sending user inputs (up & down arrows keys) from JS to the C program in the `Move` variable <br>
But what about games that runs in blocking modes? For example, those that wait for stdin inputs (like stockfish)?


#### C/C++ std::cin runing in a browser?

<div align="center">

<img src="assets/ask.png" width="150" height="150"/>

<h4>Guessing Game example</h4>
<p>Can we going to guess something in a browser?</p>

</div>

Here we going to learn how to use C/C++ std::cin related functions in browsers.

C++ loops designed to run indefinitely (e.g., game loops waiting for user input) will cause the browser to hang and eventually crash. This is because the loop prevents control from returning to the browser's event loop.

The `guess.cpp` code works on desktop but it will crash in the browser. The C++ `while loop` and the `std::cin >> userGuess;` statement blocks the browser's main thread, creating the perceived "infinite loop" problem from the browser's perspective. 

Compile with emscripten and navigate to the web page:
```sh
cd guess/hangs
emcc guess.cpp -o guess.js --std=c++17
emrun guess.html --no_emrun_detect
```

Emscripten `FS.stdin` is binded to a JS `window.popup` that is automatically fired when you try to read something with C++ `std::cin`.<br>
`Module["stdin"]` rewrites the WebAssembly FS.stdin function avoithing the window.popup. The `FS.stdin` function is low-level and handles input character-by-character. It is a callback that is invoked synchronously every time your C++ code tries to pull a character from std::cin.

Rewriting "FS.stdin" through "FS.init" in `Module.preRun` function, although it is documented, it is discouraged by the Emscripten developers and is expected to be discontinued.

At this point you could check that the program hangs due to the while loop problem commented erlier.

##### NoComm

Emscripten try to solve the "while" loop problem by telling the runtime to call a specified function periodically. `emscripten_set_main_loop()` allows the browser to handle other tasks and events between calls.

```sh
cd guess/nocomm
emcc guess.cpp -o guess.js --std=c++17  
# emcc guess1.cpp -o guess.js --std=c++17
emrun guess.html --no_emrun_detect
```

In emscripten_set_main_loop(function, int fps, int s_i_l):

`function` is a pointer to the C function that will serve as the main loop iteration. It must have a void return type and accept void as an argument. <br>
If it outputs something to std::cout, this will be printed continuosly and every time the loop runs.

`fps` is the desired number of calls per second. Setting 0 or a negative value is highly recommended for rendering applications. This uses the browser's requestAnimationFrame mechanism, which ensures smooth rendering synchronized with the monitor's refresh rate.

`s_i_l` (simulate_infinite_loop) is a boolean (0 or 1) that controls behavior after the call.<br>
If true (1), the function throws an exception in JavaScript to immediately stop execution of the calling C main() function, preventing any shutdown code from running prematurely and effectively simulating an infinite loop.<br>
If false (0), execution continues in the main() function after the call to emscripten_set_main_loop.

We use the version with a user-defined argument "void emscripten_set_main_loop_arg(function, void *arg, int fps, int s_i_l)".

But now we face another problem. The glue code is downloaded and initialized before the DOM renders so we won't find any way to write out to our page. We won't bind Module["print"] to the add_output() function.

To solve this, we're going to modularize the glue code so we can instantiate it at any time.

Obs: To override FS.stdin you must use Module["stdin"] with quotation marks. You should also clear C++ and C stream flags with `std::cin.clear()` and `std::clearerr(stdin)` after eof().


##### Runs

```sh
cd guess/runs
emcc guess.cpp -o guess.js -s ENVIRONMENT="web,worker" -s MODULARIZE=1 -s EXPORT_ES6=1 --std=c++17
emrun guess.html --no_emrun_detect
```

Compiling with `-s MODULARIZE=1` the Module variable is now a factory function that returns a promise

And thus, with the help of emscripten_set_main_loop, we have the first working version of the Guessing Game <br>
This method could be useful in games that permanently display some result or action on the console, but it introduces a delay for others that don't require interruptions and needs to perform complex calculations (like stockfish).

As we saw erlier, reading `std::cin` from the browser's main thread fails because JS cannot perform synchronous blocking I/O without hanging the entire page. To use std::cin directly you can choose between one of the following approaches:
  - Direct Communication: use Module.ccall or Module.cwrap to send strings or buffers directly to your C++ functions.
  - Pthreads + Proxy. Run your main function in a background web worker allowing the heavy blocking behavior.
  - Abandon `while(std::cin)` loop entirely.

We going to run owr C++ module in a (background) Web Worker where we can block its thread.  

#### Guessing Game. 

<div align="center">

<img src="assets/hat.png" width="150" height="150"/>

<h4>Web Workers</h4>
<h4>std::cin runing in background</h4>

</div>

Web Workers are a simple means for web content to run scripts in background threads. The worker thread can perform tasks without interfering with the user interface.

Now we can block the worker thread (we can use std::cin) without any consequence to the main browser's thread (the browser's event loop).<br>
So, we can return to use the first guess.cpp blocking C++ program.

```sh
cd guess/worker-hangs
emcc guess.cpp -o guess.js -s ENVIRONMENT=worker -s MODULARIZE=1 -s EXPORT_ES6=1 --std=c++17
emrun guess.html --no_emrun_detect
```

`ENVIRONMENT=worker` restricts the runtime environment to a worker removing calls to JS window or the DOM.<br>
Use `EXPORT_ES6` if your envirionment supports ES6 modules.
`pre.js` adds the functions needed to communicate with the WebAssembly Worker.<br>

When the worker thread is blocked by a synchronous C++ I/O operation, its event loop stops. This means it cannot process incoming postMessage() events or callbacks until the C++ operation finishes.<br>
We have verified that there is no way to send anything to the worker thread blocked by std::cin without using some interrupt method (asyncify, emscripten_set_main_loop).

```sh
cd worker
emcc guess.cpp -o guess.js -s ENVIRONMENT=worker -s MODULARIZE=1 -s EXPORT_ES6=1 --std=c++17 --bind
emrun guess.html --no_emrun_detect
```

Here we are focusing in stockfish I/O comm method. We avoid std::cin usage and run each loop iteration in aa exported one step function.<br>
The easiest way to do this is by using emscripten bindings.


#### Stockfish Wasm Communications

<div align="center">

<img src="assets/sf_128.png" width="150" height="150"/>

<h4>Focus in Stockfish I/O communication</h4>

</div>

Broadly speaking, Stockfish is state machine that holds its state in a UCI::Engine class. It returns a chess position evaluation after a movement input. In C++ it runs in a "uci_loop()" function waiting for user inputs. In browsers, we can initialize the engine and execute one loop step in each movement.

```sh
cd sf
emcc sf.cpp -o sf.js -s ENVIRONMENT=worker -s MODULARIZE=1 -s EXPORT_ES6=1 --std=c++17 --bind
emrun sf.html --no_emrun_detect
```


#### Docs

[Webassembly](/docs/webassembly.md) <br>
[Web Workers](/docs/web_workers.md) <br>
[Wasm Workers](/docs/wasm_workers.md) <br>


#### Notes

> [!Lesson] To call a function in a WebAssembly module running inside a Web Worker, you use the standard Web Worker postMessage API to send data from the main thread to the worker, which then invokes the Wasm function locally and sends the result back.

> [!Lesson] In C++, a variable that is not explicitly initialized will automativally assume whatever arbitrary value happens to be sitting in its allocated memory space. This "garbage value" can result in unpredictable bugs or crashes.

> [!Lesson] Web worker vs Service worker - While both run in background threads and cannot directly access the DOM, they serve completely different roles in a web application. The simplest way to think about it is that Web-workers are for speed and performance, while Service-worker are for reliability (network proxy) and offline features. Web-workers are not for PWA while Service-worker is Core for Push Notifications and Background Sync.

> [!Lesson] Web Workers typically do not have a dedicated entry in the console Application tab. To find them, go to the Sources tab and look for a "Threads" section.

> [!Lesson] To programmatically write to a C++ program waiting for stdin, you typically use pipes. The purpose of a pipe is to attach the stdout of one program to the stdin of another program.

> [!Lesson] The JS glue code automatically fetches and downloads the .wasm file for you in the browser using modern APIs like WebAssembly.instantiateStreaming.You do not need to manually download i to your local machine before loading the page.

> [!Lesson] C++ EAGAIN is often raised when performing non-blocking I/O. It means "there is no data available right now, try again later". It might (or might not) be the same as EWOULDBLOCK, which means "your thread would have to block in order to do that".

> [!Lesson] es6 basics: Arrow functions do not bind their own "this" context. 