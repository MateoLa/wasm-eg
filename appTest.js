'use strict'

const express = require('express');
const app = express();
const port = 5002;

const path = require('path');
app.use(express.urlencoded({extended: true}));


app.get('/', (req, res) => {
	res.sendFile(path.join(__dirname, '/test.html'));
})

app.get('/script.js', (req, res) => {
	res.sendFile(path.join(__dirname, '/script.js'));
})

app.post('/', (req, res) => {
	const nr1 = parseInt(req.body.nr1);
	const nr2 = parseInt(req.body.nr2);

	console.log("Result: ", nr1 + nr2);
//	res.sendFile(path.join(__dirname, '/test.html'));
	res.status(204).send(); // do-nothing
});


app.get('/example.wasm', (req, res) => {
	res.set('Content-Type', 'application/wasm')
	res.sendFile(path.join(__dirname, '/example.wasm'))
})

app.get('/example.js', (req, res) => {
	res.sendFile(path.join(__dirname, '/example.js'))
})

app.listen(port, () => {
	console.log("Testing WebAssembly javascript output glue code. Port: ", port)
})

