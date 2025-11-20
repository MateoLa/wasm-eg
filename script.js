
////// Primera Forma de instanciar WebAssembly 
////// Usando el "pegamento" (js glue code) que genera el compilador emscripten (emcc). 


import Module from './example.js';

let engine = Module();

/*
let res = document.getElementById("res");
let greet = document.getElementById("greet");

document.getElementById("form").addEventListener("submit", (event) => {
    let nr1 = document.getElementById("nr1").value;
    let nr2 = document.getElementById("nr2").value;
    
        res.textContent = add(nr1, nr2);
        greet.textContent = getText();
});

function getText() {
    const bytes = new Int8Array(
        memory.buffer,  // Wasm memory 
        greet(),        // string pointer
        32              // string length
    );
    let string = new TextDecoder('utf8').decode(bytes);
    console.log(string);
    return string;
}
*/


////// Segunda Forma de instanciar WebAssembly 
////// Instanciar directamente el codigo wasm compilado.

/*
let engine = await WebAssembly.instantiateStreaming(fetch('/example.wasm'));
*/

let res = document.getElementById("res");
let greet = document.getElementById("greet");

document.getElementById("form").addEventListener("submit", (event) => {
    let nr1 = document.getElementById("nr1").value;
    let nr2 = document.getElementById("nr2").value;
    res.textContent = engine.instance.exports.add(nr1, nr2);
    greet.textContent = getText();
});


function getText() {
    const bytes = new Int8Array(
        engine.instance.exports.memory.buffer,  // Wasm memory 
        engine.instance.exports.greet(),        // string pointer
        32                                      // string length
    );
    let string = new TextDecoder('utf8').decode(bytes);
    console.log(string);
    return string;
}
