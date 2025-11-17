<div align="center">

	![WebAssembly]<img src=./wasm4.svg width="150" height="150"></img>]

	<h3>WebAssembly</h3>

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
If you want to generate example.js and example.wasm from example.c by your own. 
`emcc example.c -o example.js 

### Run

cd into wasm-eg directory.

```js
node appTest.js
```

Navigate to localhost:5002
