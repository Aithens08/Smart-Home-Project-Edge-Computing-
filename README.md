# Smart-Home-Project-Edge-Computing-
DIY Smart home project developed for the Edge Computing classe

## Replacing Bluetooth connection
As I encountered difficulties in maintaining a consistent and stable link between the Arduino and the computer using the Bluetooth module, I have made the decision to replace the Bluetooth connection with a more reliable traditional serial connection.

## Connexion to the home server
We have an Arduino that periodically emits sensor data. To handle this task, I have decided to employ Node.js as the technology for our web server. Node.js is well-suited for building scalable and efficient web applications, making it an ideal choice in this scenario.

For storing the incoming sensor data, I have chosen MongoDB as the database solution. MongoDB is a NoSQL database that offers flexibility and scalability, making it suitable for handling large amounts of data generated by our sensors.

To visualize the sensor data and provide a user-friendly interface for controlling the motors, I will be utilizing HTML and JavaScript modules, specifically leveraging Chart.js. Chart.js is a powerful JavaScript library that enables the creation of interactive and visually appealing charts, making it perfect for displaying sensor data in a web portal.

By combining Node.js for the web server, MongoDB for the database, and HTML along with JavaScript modules like Chart.js for the web portal, we can create a comprehensive and efficient system for visualizing sensor data and controlling the motors.

## Listening for Incoming Serial Transmissions in Node.js
In order to be able to listen for incoming serial transmissions in Node.js, you'll need to follow these steps:

### Step 1: Install the Required Dependencies
First, make sure you have Node.js installed on your system. You can download and install Node.js from the official website: https://nodejs.org

Once Node.js is installed, create a new Node.js project. This need to be a fonctional web server, in practice it is comon to use the localhost:3000 adress for the webserver. To build a basic web server you'll need a app.js file, a package.json file and an index.html file to access the web server via a browser.

Into the app.js file add this code to be able to run a web server on your computer :
```ruby
var http = require('http');
var fs = require('fs');

var index = fs.readFileSync('index.html');

var app = http.createServer(function (req, res){
   res.writeHead(200, {'Content-Type':'text/html'});
   res.end(index);
});

app.listen(3000)
```
You can start the web server by runing the following command : 
```ruby
node app.js
```
If everything is set up correctly, the server will start running, and you can access it by navigating to http://localhost:3000 in your web browser.

Next, install the serialport package, which will allow us to communicate with the serial port:
```ruby
npm install serialport
```
And add this package to the dependecies in the package.json file.

### Step 2: Set Up the Serial Connection

Inside the app.json we can start to build the application that will listen for incoming data : 
```ruby
var SerialPort = require("serialport"); //importing the relevant package


const parsers = SerialPort.parsers; //defining a parser to parse the incoming serial data

const parser = new parsers.Readline({
   delimiter: '\r\n'
});

var port = new SerialPort('COM5',{
    baudRate: 9600,
}); //defining the communication port for the serial communication


port.pipe(parser); //attach the cmmunication port to the parser
```
### Step 3: Set Up the Serial Listener
Once everything is correctly set-up, we can add a listener that can execute code when the data event is catched. The data event is triggered everytime data is received via the defined serial port.
```ruby
parser.on('data', function(data){
  //execute code when data is received throug serial communication
});
```
## Storing data in a database with MongoDB
With a MongoDB server running (It could be cloud, locally installed on your computer or running in a docker container) you need to connect it to the node.js server to be able to store data in the database. The connection is fairly simple with the mongodb package for JavaScript. In the app.js file add the following code :
```ruby
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27012"; //this is the adress of your mongodb server

const client  = new MongoClient(url);
```

Now using the Serial listener that we set-up before we can store incoming data everytime that we receive new data. 

```ruby
async function run(data){ //function to store incoming data to the database
    const database = client.db("SmartHomeHub"); //select the desired database
    const sensordata = database.collection("sensordata"); //select the desired collection in the selected database
    var result = await sensordata.insertOne(data); //insert the json data in the database
};

parser.on('data', function(data){
    var jsondata = JSON.parse(data) //transfrom incoming string into json 
    jsondata["Timestamp"] = new Date().toUTCString(); //add a new field in the json to store the time of the data
    run(jsondata); //call the function that will insert the data into the database
});
```

The same way, we can build function to retrieve data (usefull for visualizing the sensor data in the web page).
```ruby
async function retrieve(){
        const database = client.db("SmartHomeHub");
        const sensordata = database.collection("sensordata");
        const finddoc = await sensordata.find().project({_id: 0,light_value: 1, temp_value:1}).limit(200).sort({Timestamp:-1});
        var retrieved_data = [];
        for await (const doc of finddoc) { // replace the pointer received from the find() function into an array of data
            retrieved_data.push(doc);
        }
        return retrieved_data
};

```

## Building a web interface
Finally the last step but non the least is to build a web interface to be able to visualize and interact with the sensors and the motors.

To enable communication between the web server and the web portal, you can use the Socket.IO package, which allows for emitting and listening to custom events. For exemple in the app.js you can have :
```ruby
var io = require("socket.io").listen(app);

io.on('connection', function(socket){
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
});
};
```

And in the index.html :
```ruby
 <script>
        var socket = io.connect('/');
        var fan_status;
        var curtain_status;

        socket.on('data', function(data){
            fan_status = data["fan_status"];
            curtain_status = data["curtain_status"];
        });

        const fanbutton = document.getElementById('fan');
        fanbutton.addEventListener('click', function(e) {
            if (fan_status == 0){
                socket.emit("turnOnFan");
            } else if (fan_status == 1 ){
                socket.emit("turnOffFan");
            } else {
                console.log("Fan status unknown");
            }

        });

        const curtainbutton = document.getElementById('curtain');
        curtainbutton.addEventListener('click', function(e) {
            if (curtain_status == 0){
                socket.emit("turnOnCurtain");
            } else if (curtain_status == 1 ){
                socket.emit("turnOffCurtain");
            } else {
                console.log("Curtain status unknown");
            }

        });
    </script>
```

Finally using socket.io we can pass to the web interface the data that we retreive from the database. We can then use Chart.js to plot the data:
```ruby
function range(start, end) {
            return Array(end - start + 1).fill().map((_, idx) => start + idx)
        }

        socket.on('update', function(light_data){
            var light_plot = [];
            var temp_plot = []
            for (const element of light_data) {
                light_plot.push(element['light_value']);
                temp_plot.push(element['temp_value']);
            }
            const ctx1 = document.getElementById('lightChart');
            new Chart(ctx1, {
                type: 'line',
                data: {
                    labels: range(0,200),
                    datasets: [{
                        label: 'Light Sensor Values',
                        data: light_plot.reverse(),
                        fill: false,
                        borderWidth: 1,
                        borderColor: '#36A2EB',
                        backgroundColor: '#9BD0F5',
                    }]
                }
            });
            const ctx2 = document.getElementById('tempChart');
            new Chart(ctx2, {
                type: 'line',
                data: {
                    labels: range(0,200),
                    datasets: [{
                        label:'Temperature Sensor Values',
                        data: temp_plot.reverse(),
                        fill:false,
                        borderWidth: 1,
                        borderColor: '#FF6384',
                        backgroundColor: '#FFB1C1',
                    }]
                },
            });
        });
```
This is a visual of the final web interface, with the buttons for controling the motors and the graphs to visualize the stored data.
![Capture d’écran 2023-06-05 175932](https://github.com/Aithens08/Smart-Home-Project-Edge-Computing-/assets/71881774/f665b59e-98c1-41be-86da-c0915772edbc)



