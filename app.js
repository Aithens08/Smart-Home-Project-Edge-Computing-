var http = require('http');
var fs = require('fs');
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27012";

const client  = new MongoClient(url);

var index = fs.readFileSync('index.html');

var SerialPort = require("serialport");


const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
   delimiter: '\r\n'
});

var port = new SerialPort('COM5',{
    baudRate: 9600,
});


port.pipe(parser);

var app = http.createServer(function (req, res){
   res.writeHead(200, {'Content-Type':'text/html'});
   res.end(index);
});


var io = require("socket.io").listen(app);

async function retrieve(){
        const database = client.db("SmarHomeHub");
        const sensordata = database.collection("sensordata");
        const finddoc = await sensordata.find().project({_id: 0,light_value: 1, temp_value:1}).limit(200).sort({Timestamp:-1});
        var light_data = [];
        for await (const doc of finddoc) {
            light_data.push(doc);
        }
        return light_data
};

async function run(data){
    const database = client.db("SmarHomeHub");
    const sensordata = database.collection("sensordata");
    var result = await sensordata.insertOne(data);
};

io.on('connection', function(socket){
    console.log("NodeJS is listening on port 3000");
    retrieve().catch(console.dir).then(result => io.emit('update', result));
    socket.on("turnOnFan", function (){
        console.log("Turning fan on");
        port.write('1:1');
    });
    socket.on("turnOffFan", function (){
        console.log("Turning fan off")
        port.write('1:0');
    });
    socket.on("turnOnCurtain", function (){
        console.log("Turning curtain on");
        port.write('2:1');
    });
    socket.on("turnOffCurtain", function (){
        console.log("Turning curtain off")
        port.write('2:0');
    });
});

parser.on('data', function(data){
    var jsondata = JSON.parse(data)
    jsondata["Timestamp"] = new Date().toUTCString();
    io.emit('data', jsondata);
    run(jsondata).catch(console.dir);
    retrieve().catch(console.dir).then(result => io.emit('update', result));
});



app.listen(3000)