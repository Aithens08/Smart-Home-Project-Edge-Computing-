# Smart-Home-Project-Edge-Computing-
DIY Smart home project developed for the Edge Computing classe

## Bluetooth comunication between Arduino and Android
Initially, in order to configure the Bluetooth connection, I only had my Android phone as a device to test and set up the connection. Therefore, my first task was to establish a two-way connection between the Arduino and the Android phone.

In order for the Arduino to send sensor data via Bluetooth every X minutes and receive commands from the Android device at any time, a challenge arises as using a `delay()` command stops all code from running. To overcome this issue, I employed an if statement inside the void loop. This if statement permits the code inside it to be executed at every time step specified by the user (e.g. 1min in the example). By using this technique, we can continuously listen for incoming commands in the void loop and simultaneously send sensor data every X minutes.

```ruby
unsigned long previousMillis = 0;
const long interval = 60000;

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (mySerial.available())
  {
    String receivedString = mySerial.readString();
    Serial.println(receivedString);
  }

  if (currentMillis - previousMillis >= interval) {
    // save the last time you ran the code
    previousMillis = currentMillis;

    // put your code here that needs to run every minute

    if (Serial.available()){
      while (Serial.available() > 0)
        mySerial.write(Serial.read());
    }
  }
}
```
### Sending sensor data
To send sensor data from an Arduino via Bluetooth, we can make use of the `SoftwareSerial` library in Arduino. First, we need to initialize a new `SoftwareSerial` object in the Arduino code that is connected to the Bluetooth module. We can then read the sensor data in the Arduino code and send it over the serial port using the `Serial.write()` function. For instance, we can read an analog sensor connected to analog pin 0 and send its value over Bluetooth every 1 second using the following code:
```ruby
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3); // RX, TX pins of Bluetooth module

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  // read sensor data
  int sensorValue = analogRead(A0);

  // send sensor data via Bluetooth
  bluetooth.write(sensorValue);

  delay(1000); // wait for 1 second
}
```

As part of this project, my goal was not only to send sensor data, but also the current state of the different devices controlled by the Arduino. In order to simplify communication and data formatting, I used the ArduinoJson library to format the data and enclose it within a JSON file. The following example illustrates how this was done:
```ruby
#include <ArduinoJson.h>

void loop() {
// create a new JSON object
    StaticJsonDocument<200> doc;
    doc["light_value"] = light;
    doc["temp_value"] = temp_celsius;
    doc["fan_status"] = fan_status;
    doc["curtain_status"] = curtain_status;

    // serialize the JSON object to a string
    String jsonString;
    serializeJson(doc, jsonString);

    // send the JSON string to the Bluetooth module
    BTSerial.println(jsonString);
}
```

### Receiving commands
In order to control the different sensors through Bluetooth, I implemented a cascade of `if/else` statements along with a particular communication format. I chose to format the commands in the following way: *SensorID:State*. In this project, I had two sensors, so I arbitrarily assigned ID numbers 1 and 2, and for the states, I used *"open"* or *"closed"*, represented as 1 or 0 respectively. For instance, a proper command to turn on the fan using an Android device could be to send 2:1.

To implement this communication format, I first read the incoming Bluetooth data using the `SoftwareSerial.readString()` function in the Arduino code. Finally, I used a cascade of if/else statements with the `String.charAt()` function to check which sensor was being referred to and which state was being set. Here is an example code snippet that demonstrates this:


```ruby
void loop() {
  if (BTSerial.available())
  {
    String receivedString = BTSerial.readString();
    if (receivedString.charAt(0) == '1')
    {
      if (receivedString.charAt(2) == '0')
      {
        fan_status = 0;
        digitalWrite(fan_pin, LOW); // turn off
      }
      else
      {
        if (receivedString.charAt(2) == '1')
        {
          fan_status = 1;
          digitalWrite(fan_pin, HIGH); // turn on
        }
      }
    }
    else
    {
      if (receivedString.charAt(0) == '2')
      {
        if (receivedString.charAt(2) == '0')
        {
          curtain_status = 0;
          Servo1.write(0);
        }
        else
        {
        if (receivedString.charAt(2) == '1')
          {
            curtain_status = 1;
            Servo1.write(180);
          }
        }
      }
    }
  }
```
## Actual state of the project
The current version of this project allows for the reading of sensor data and sending it over Bluetooth, as well as receiving commands from an Android device to control the state of a fan and a servo motor.
## Next Steps
The next step in this project is to replace the Android phone with an old laptop that can be reused as a home server. This home server will serve as a compute node for the sensor data, allowing for more complex data analysis and visualization. The laptop will be connected to the Arduino via Bluetooth and will receive the sensor data, process it, and store it for later analysis. With the additional computing power of the laptop, we can perform more advanced analysis on the sensor data, such as machine learning algorithms for predictive maintenance or anomaly detection. This will enable the project to evolve from a simple sensor monitoring system to a more sophisticated and intelligent system that can provide valuable insights and optimize the operation of the monitored devices.

