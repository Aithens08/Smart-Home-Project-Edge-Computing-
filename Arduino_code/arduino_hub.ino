#include <ArduinoJson.h>
#include <Wire.h>
#include <Servo.h> 

const int light_sensor_address = 0x23; // Set the I2C address of the light sensor
const int temp_sensor_address = 0x48;  // Set the I2C address of the temperature sensor

const int fan_pin = A5; // Arduino pin for the fan
const int servo_pin = 3; // Arduino pin for the servo 

Servo Servo1; // Create the servo Object

float temp_celsius;
int light;
int fan_status = 0;
int curtain_status = 0;
unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // Initialize the I2C bus
  Wire.begin();

  pinMode(fan_pin, OUTPUT); // initialize digital pin as an output for the fan
  Servo1.attach(servo_pin); // Attach the servo to the correct pin of the arduino
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (Serial.available())
  {
    String receivedString = Serial.readString();
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

  if (currentMillis - previousMillis >= interval) {
    // save the last time you ran the code
    previousMillis = currentMillis;
  
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
    }

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
    Serial.println(jsonString);
  }
}
