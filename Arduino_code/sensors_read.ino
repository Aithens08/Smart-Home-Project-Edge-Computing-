#include <Wire.h>
#include <Servo.h> 

const int light_sensor_address = 0x23; // Set the I2C address of the light sensor
const int temp_sensor_address = 0x48;  // Set the I2C address of the temperature sensor

const int temp_threshold_upper = 24; // upper threshold of temperature
const int temp_threshold_lower = 23; // lower threshold of temperature
const int light_threshold = 500; // Light treshold

const int fan_pin = A5; // Arduino pin for the fan
const int servo_pin = 3; // Arduino pin for the servo 

Servo Servo1; // Create the servo Object

float temp_celsius;
int light;

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize the I2C bus

  pinMode(fan_pin, OUTPUT); // initialize digital pin as an output for the fan
  Servo1.attach(servo_pin); // Attach the servo to the correct pin of the arduino
}

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

  // Read temperature sensor data
  byte temp_data[2];
  Wire.beginTransmission(temp_sensor_address); // Start communication with the temperature sensor
  Wire.write(0x00); // Send command to request temperature data
  Wire.endTransmission();

  delay(200); // Wait for the sensor to process the command

  Wire.requestFrom(temp_sensor_address, 2); // Request 2 bytes of data from the temperature sensor

  if (Wire.available() == 2) {
    temp_data[0] = Wire.read(); // Read the first byte
    temp_data[1] = Wire.read(); // Read the second byte
    int temp = ((temp_data[0] << 8) | temp_data[1]); // Combine the two bytes to get the temperature
    temp_celsius = (temp / 256.0); // Convert the temperature to Celsius
    Serial.print("Temperature: ");
    Serial.print(temp_celsius);
    Serial.println(" C"); // Print the temperature to the serial monitor
  }

  if(temp_celsius >= temp_threshold_upper){
    Serial.println("The fan is turned on");
    digitalWrite(fan_pin, HIGH); // turn on
  } else if(temp_celsius < temp_threshold_lower){
    Serial.println("The fan is turned off");
    digitalWrite(fan_pin, LOW); // turn on
  }

  if(light >= light_threshold){
    Serial.println("The curtain is open");
    Servo1.write(0);
  } else {
    Serial.println("The curtain is closed");
    Servo1.write(180);
  }


  delay(2000); // Wait for 1 second before requesting data again
}
