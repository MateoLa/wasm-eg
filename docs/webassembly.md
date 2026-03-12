### Compiling C/C++ to WebAssembly

Browsers don't know how to run C code.<br>
WebAssembly is a W3C standard to facilitate high-performance applications on web pages.<br>
You can compile C/C++ into WebAssembly using a tool like Emscripten.

hello.cpp:
```c
#include <iostream>

int main() {
    std::cout << "Greetings from MaLa!!\n";
    return 0;
}
```

```sh
emcc hello.cpp -o hello.html --emrun
```

At tshis point in your source directory you should have:
  * The binary Wasm module code: hello.wasm
  * A hello.js file containing the glue code to link JS to the native C functions.
  * An HTML file to load, compile, and instantiate your Wasm code in the browser: hello.html

Read the comments at the begining of the JS glue code to learn how to use the `Module` variable.<br>

Emrun is a local web sever and test tool used to host and launch the compliled html and WebAssembly files.

```sh
emrun hello.html
```

### Calling C++ funcitons from JavaScript

Emscripten requires a large variety of JavaScript "glue" code to handle memory allocation, memory leaks, and a host of other problems.

```sh
emcc -o hello.js hello.c -O3
```

Alternatively, you can produce a factory module to work with node.js.


```sh
emcc -o hello.js hello.c -O3 -sMODULARIZE -sEXPORT_ES6
```

You could then incorporate this JavaScript file into your program.<br>
In your app's entry module:

```js
import "./hello.js";
```

This also enables to produce multiple instances of the module. By default the glue code loads the module globally, causing multiple instances to collide.

If your output extension is .js and not .mjs, then you have to add `-s EXPORT_ES6` to output a JavaScript module.


### Calling C/C++ custom functions

To call a function defined in C/C++ from JavaScript you can:
  * Use Emscripten ccall() function and the EMSCRIPTEN_KEEPALIVE declaration
  * Use Emscripten embind and the EMSCRIPTEN_BINDINGS declaration

This two alternatives adds the functions to the JS glue code exported functions list.

```c
#include <stdio.h>
#include <emscripten/emscripten.h>

int main() {
    printf("Hello World\n");
    return 0;
}

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE void myFunction(int argc, char ** argv) {
    printf("MyFunction Called\n");
}
```

By default Emscripten generated code calls the main() function and other functions are eliminated as dead code.<br>
Adding EMSCRIPTEN_KEEPALIVE before a function name stops this from happening. 

We are including the `#ifdef block` so that if you are trying to include this in C++ code, the example will still work. Due to C versus C++ name mangling rules, this would otherwise break. If you are using C++ the code will be treated as an external C function.

```sh
emcc -o hello.js hello.c -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -s MODULARIZE -s EXPORT_ES6
```

* Note that we need to compile with NO_EXIT_RUNTIME: otherwise, when main() exits, the runtime would be shut down and it wouldn't be valid to call compiled code. This is necessary for proper C emulation: for example, to ensure that atexit() functions are called.

You need to run the new myFunction() function from JavaScript. 

Add a <button> element to your html.

```html
<button id="my-button">Run myFunction</button>
```
Add the following code at the end of the first <script> element:

```js
document.getElementById("my-button").addEventListener("click", () => {
  alert("check console");
  const result = Module.ccall(
    "myFunction", // name of C function
    null, // return type
    null, // argument types
    null, // arguments
  );
});
```
This illustrates how ccall() is used to call the exported function.


#### Note

Extracted from https://developer.mozilla.org/en-US/docs/WebAssembly/Guides/C_to_Wasm

