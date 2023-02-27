# Smart-Home-Project-Edge-Computing-
DIY Smart home project developed for the Edge Computing classe

### Using sensors, motors and servo with Arduino

On the first day I build a simple Arduino script using I2C protocol to retrieve data from several sensors.

Using the I2C protocol with Arduino is very easy and requires only a few steps. First, you need to include the Wire.h library, which contains easy-to-use functions for I2C communication. Then you need to define the I2C address of each sensor, this is mandatory for the Arduino to know which address to listen to for information.

```ruby
#include <Wire.h>

const int light_sensor_address = 0x23; // Set the I2C address of the light sensor
const int temp_sensor_address = 0x48;  // Set the I2C address of the temperature sensor
```

Once the addresses are set, we can tell the Arduino to start an I2C connection using functions from the Wire.h library.

```ruby
void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize the I2C bus
}
```

Finally, we can request data from a sensor (light sensor in the example), read it and, in this case, print it on the serial monitor.

```ruby
void loop() {
  // Read light sensor data
  byte light_data[2];
  Wire.beginTransmission(light_sensor_address); // Start communication with the light sensor
  Wire.write(0x10); // Send command to request data
  Wire.endTransmission();

  delay(200); // Wait for the sensor to process the command

  Wire.requestFrom(light_sensor_address, 2); // Request 2 bytes of data from the light sensor

  if (Wire.available() == 2) {
    light_data[0] = Wire.read(); // Read the first byte
    light_data[1] = Wire.read(); // Read the second byte
    light = ((light_data[0] << 8) | light_data[1]); // Combine the two bytes to get the light level
    Serial.print("Light level: ");
    Serial.println(light); // Print the light level to the serial monitor
  }
  
  delay(1000); // Wait 1s before requesting data again
  }
```

The following week I added a fan and a servo motor to the project. The aim of these add-ons was to simulate a motorised curtain (for the servo) and an autonomously controlled temperature environment (with the fan). Because we already have access to the sensors information, adding these elements is quite simple. Once they are connected to the board, all they need is a simple if/else statement with an appropriate threshold to turn them on and off when needed.

```ruby
  if(temp_celsius >= temp_threshold_upper){
    Serial.println("The fan is turned on");
    digitalWrite(fan_pin, HIGH); // turn on
  } else if(temp_celsius < temp_threshold_lower){
    Serial.println("The fan is turned off");
    digitalWrite(fan_pin, LOW); // turn on
  }
```

### Actual state of the project

To show the current state of the project, here is a picture of the actual hardware on my desk and the corresponding schematic.

![20230227_094704_2](https://user-images.githubusercontent.com/71881774/221547548-3d99825b-8ea7-433c-85b9-ee5b71362da9.jpg)

![Tuto2IMG](https://user-images.githubusercontent.com/71881774/221546435-f4282890-128a-4c31-94cf-28e0934fe0bb.png)

### Next Steps

As you can see from the schematic, a Bluetooth module has been added to the project. The next steps are to use the Bluetooth module to communicate with the Arduino.
