import xx from "./sf.js"

let engine;

let Module = {
    print: (text) => { self.postMessage(text) },
    printErr: (err) => { console.warn("MaLa C++ error: ", err); },
    onRuntimeInitialized: function() { console.log('Module loaded: ', Module); }
};

// Initialize the module with your options xx(options)
xx(Module).then((instance) => { engine = instance });

self.onmessage = (e) => { engine.wasm_uci_execute(e.data); }


