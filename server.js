var http = require("http");
//var url = require("url");
var SerialPort = require("serialport").SerialPort;

var ledcube = new SerialPort("/dev/ttyACM1", {
    baudrate: 9600,
    dataBits: 8, 
    parity: 'none', 
    stopBits: 1, 
    flowControl: false 
});

var ledcubeReady = false;

ledcube.on("open", function(){
    console.log('ledcube connected');
    setTimeout(cubeReady , 2000);
});

function cubeReady() {
	ledcubeReady = true;
	ledcube.on('data', function getData(data) { 
		receivedData = data.toString();
		console.log(receivedData);
		});

}

function mainLoop() {
  console.log('open');
}


function onRequest(request, response) {
  //var pathname = url.parse(request.url).pathname;
  //console.log("url " + request.url);
  
  
  
  var bits = request.url.replace(/[^01]/g, "");
  
  if(bits.length==64) {
	console.log("64 bits baby");  
	
	console.log("bits " + bits);
	
	var bytes = bits.match(/.{8}/g).map(function(x) { return String.fromCharCode( parseInt(x, 2) ); }).join("");
	
	console.log("grouped " + bits.match(/.{8}/g).map(function(x) { return parseInt(x, 2); }) );
	console.log("bytes " + bytes);
	
	ledcube.write(bytes);
	
	if(bytes.length!=8) {console.log("no 8 bytes!!");}
	
	response.writeHead(200);
  } else {
	response.writeHead(404, {"Content-Type": "text/plain"});
	response.write("request should contain 64 bits, now contains "+bits.length);
  }
  
  //input.match(/.{2}/g).map(function(x) { return parseInt(x, 16); })
  
  response.end();
}

http.createServer(onRequest).listen(8000);
console.log("server listening on port 8000");
