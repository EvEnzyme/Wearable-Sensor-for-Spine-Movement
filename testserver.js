const net = require('net');

let testserver = net.createServer((socket) => {
	socket.on('data', (data) => {
		console.log("Received: " + data);
	});
	console.log("Accepted connection.");
	socket.write("Hello from the server!\n");
}).listen(10200, '127.0.0.1', () => console.log("Listening on 10200."));
