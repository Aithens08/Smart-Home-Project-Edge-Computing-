#include <Wire.h>

const int light_sensor_address = 0x23; // Set the I2C address of the light sensor
const int temp_sensor_address = 0x48;  // Set the I2C address of the temperature sensor

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Initialize the I2C bus
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
    int light = ((light_data[0] << 8) | light_data[1]); // Combine the two bytes to get the light level
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
    float temp_celsius = (temp / 256.0); // Convert the temperature to Celsius
    Serial.print("Temperature: ");
    Serial.print(temp_celsius);
    Serial.println(" C"); // Print the temperature to the serial monitor
  }

  delay(1000); // Wait for 1 second before requesting data again
}
