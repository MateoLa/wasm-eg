import xx from "./guess.js"
/*
var Module = {
    stdinBuffer: {
        str: "",
        index: 0,
        set: function(str) {
            this.str = str + "\n";
            this.index = 0;
        }
    },
    stdin: function() {
        let code = stdinBuffer.str.charCodeAt(stdinBuffer.index++);
        console.log(code);
        return isNaN(code) ? null : code;
    },
    print: (text) => {
        self.postMessage("Warningggggg");
        self.postMessage(text); },
    printErr: (err) => { console.warn("MaLa error: ", err); },

    onRuntimeInitialized: () => { console.log('Module loaded: ', Module); },
}
*/

// Initialize the module with your options
xx().then((instance) => {
  // Use your module instance here
  console.log("Module fully loaded and ready.");
  self.postMessage("DonKing");
});

/*
let guessModule;

// Load the promise into a variable
// Use with precaution: any print before the promise finishes will fail
myModule().then((instance) => {
  // Use your module instance here
  console.log("Module fully loaded and ready.");
  guessModule = instance;
});
*/

self.onmessage = (e) => {
    self.postMessage("Jelooouuuu");
//    guessModule.stdinBuffer.set(e.data);
}



