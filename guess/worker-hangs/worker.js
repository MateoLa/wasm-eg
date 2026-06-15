import xx from "./guess.js"


var Module = {
    "stdin": function() {
        let ascii;
        self.onmessage = (e) => { ascii = e.data.charCodeAt(0); };
        return isNaN(ascii) ? null : ascii;
    },
    print: (text) => { self.postMessage(text) },
    printErr: (err) => { console.warn("MaLa wasm error: ", err); },
    onRuntimeInitialized: function() { console.log('Module loaded: ', Module); }
};


// Initialize the module with your options xx(options)
xx(Module).then((instance) => {
    // Use your module instance here
    console.log("Module fully loaded and ready.");
});


self.onmessage = (e) => { Module.stdin(e.data.charCodeAt(0)); };



