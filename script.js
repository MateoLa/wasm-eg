// Primera Forma de trabajar WebAssembly es importando el codigo 
// de "pegamento" o js glue code que genera el compilador emscripten (emcc). 

import modulo from './example.js';

let response = document.getElementById("output");

document.getElementById("form").addEventListener("submit", (event) => {
    let nr1 = document.getElementById("nr1").value;
    let nr2 = document.getElementById("nr2").value;
    modulo().then(obj => response.textContent = obj._add(nr1, nr2));
})


/*

// Segunda Forma de trabajar WebAssembly es instanciar directamente el 
// codigo wasm que genera el compilador emscripten (emcc).

let engine = await WebAssembly.instantiateStreaming(fetch('/example.wasm'));

let response = document.getElementById("output");

document.getElementById("form").addEventListener("submit", (event) => {
    let nr1 = document.getElementById("nr1").value;
    let nr2 = document.getElementById("nr2").value;
    response.textContent = engine.instance.exports.add(nr1, nr2);
});

*/

