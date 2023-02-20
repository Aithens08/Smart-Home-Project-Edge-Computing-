#include <Wire.h>

int LightSensorAddress = 0x23; // Device address in which is also included the 8th bit for selecting the mode, read in this case.
int TmpSensorAdress = 0x48;

int Light;

void setup() {
  Wire.begin(); // Initiate the Wire library
  Serial.begin(9600);
  delay(100);
  // Enable measurement
  Wire.beginTransmission(LightSensorAddress);
  // Bit D3 High for measuring enable (0000 1000)
  Wire.write(8);  
  Wire.endTransmission();
}

void loop() {
  Wire.beginTransmission(LightSensorAddress); // Begin transmission to the Sensor 
  //Ask the particular registers for data
  Wire.write(0x23);
  
  Wire.endTransmission(); // Ends the transmission and transmits the data from the two registers
  
  Wire.requestFrom(0x23,120); // Request the transmitted two bytes from the two registers
  
  if(Wire.available()<=120) {  // 
    Light = Wire.read(); // Reads the data from the register  
  }
  
  Serial.print("\n Light sensor= ");
  Serial.print(Light);
  delay(1000);
}