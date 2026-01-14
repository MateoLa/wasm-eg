<div align="center">

<img src=wasm4.svg width="150" height="150"/>

<h3>Running C++ code into a Web Worker</h3>

<p>C++ WebAssembly compilation example</p>

</div>


#### WebAssembly 

Wasm is a binary instruction format for a stack-based virtual machine.

The code can be run in a modern browser to build a virtual machine which allows developers to run compiled codes (C++, Rust, etc.) on the client.

WebAssembly is designed to complement and run alongside JavaScript, sharing functionality between them.


### Prerequisites

* Install Node
* Install Express (npm install express)


* Install Emscripten
(If you want to compile to .wasm and generate the .js "glue" code)

```sh
emcc example.cpp -o example.js -s EXPORTED_FUNCTIONS="['_add', '_greet']" -s MODULARIZE=1  --> ?

emcc example.cpp -o example.js -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -s MODULARIZE -s EXPORT_ES6  --> ?
```


### Run

cd into wasm-eg directory and run:

```js
node app.js
```

Navigate to localhost:5002


### Docs

[Webassembly](/docs/webassembly.md)
[Web Workers](/docs/web_workers.md)
[Wasm Workers](/docs/wasm_workers.md)
