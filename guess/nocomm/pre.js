// const data = 'Hello\nworld!\n'.split('').map(c => c.charCodeAt(0));
// Module['stdin'] = () => data.shift() || null;

var stdinBuffer = {
    str: "Hello\nWorld!\n",
    index: 0,
    set: function(str) {
        this.str = str + "\n";
        this.index = 0;
    },
}

Module['stdin'] = () => {
    let code = stdinBuffer.str.charCodeAt(stdinBuffer.index++);
    return isNaN(code) ? null : code;
};