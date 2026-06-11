import xx from "./guess.js"


var stdinBuffer = {
    str: "Hello\nWorld!\n",
    index: 0,
    set: function(str) {
        this.str = str + "\n";
        this.index = 0;
    }
};
var Module = {
    "stdin": function() {
        let code = stdinBuffer.str.charCodeAt(stdinBuffer.index++);
        return isNaN(code) ? null : code;
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

self.onmessage = (e) => {
    stdinBuffer.set(e.data);
}


