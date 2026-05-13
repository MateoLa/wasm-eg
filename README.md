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
cd start
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


#### C/C++ Game runing in a browser

<div align="center">

<img src="assets/ask.png" width="150" height="150"/>

<h4>Guessing Game example</h4>

</div>

C++ loops designed to run indefinitely (e.g., game loops waiting for user input) will cause the browser tab to hang and eventually crash. This is because the loop prevents control from returning to the browser's event loop.

```sh
cd hangs
emcc guess.cpp -o guess.js --std=c++17
emrun guess.html --no_emrun_detect
```

The `guess.cpp` code works on desktop but it will crash in the browser. The `std::cin >> userGuess;` statement inside the while loop blocks the main thread, creating the perceived "infinite loop" problem from the browser's perspective. 

Emscripten solves this by telling the runtime to call a specified function periodically. `emscripten_set_main_loop()` allows the browser to handle other tasks and events between calls. <br>
Another option is to use `Asyncify` which will rewrite the program so that it can return to the browser's main event loop by just calling "emscripten_sleep()".

```sh
cd runs
emcc guess.cpp -o guess.js --std=c++17
emrun guess.html --no_emrun_detect
```

* void emscripten_set_main_loop(function, int fps, int simulate_infinite_loop);

or the version with a user-defined argument:

* void emscripten_set_main_loop_arg(function, void *arg, int fps, int simulate_infinite_loop);

`function` is a pointer to the C function that will serve as the main loop iteration. It must have a void return type and accept void (or void* for the _arg version) as an argument.

`fps` is the desired number of calls per second. Setting 0 or a negative value is highly recommended for rendering applications. This uses the browser's requestAnimationFrame mechanism, which ensures smooth rendering synchronized with the monitor's refresh rate.

`simulate_infinite_loop`: A boolean (0 or 1) that controls behavior after the call.<br>
If true (1), the function throws an exception in JavaScript to immediately stop execution of the calling C main() function, preventing any shutdown code from running prematurely and effectively simulating an infinite loop.<br>
If false (0), execution continues in the main() function after the call to emscripten_set_main_loop.

Obs: when emscripten_set_main_loop() runs, it yields execution back to the browser's event loop on every frame. If you attempt a `cin >> x` inside the "function", it will immediately read whatever is in the stdin buffer (often an empty string or a newline character) without actually waiting for the user to type anything. To solve this one option is to flush the cin buffer (clear & synch).

`FS.stdin` is a callback that is invoked synchronously every time your C++ code tries to pull a character from std::cin.


#### Memory Out of Bounds error<br>

Here we force a JS "RuntimeError: memory access out of bounds" error.

```sh
cd loop
emcc loop.cpp -o loop.js --std=c++17 --bind
emrun loop.html --no_emrun_detect
```


#### Docs

[Webassembly](/docs/webassembly.md) <br>
[Web Workers](/docs/web_workers.md) <br>
[Wasm Workers](/docs/wasm_workers.md) <br>
