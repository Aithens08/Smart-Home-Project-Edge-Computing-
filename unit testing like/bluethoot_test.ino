#include <SoftwareSerial.h>
#define bluetoothRX 50
#define bluetoothTX 51
SoftwareSerial mySerial(bluetoothRX, bluetoothTX);
String message; //Message buffer

char character; 
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Open serial communication for the Bluetooth Serial Port
  mySerial.begin(115200);
}

void loop() {
  // If any data is available at the Bluetooth Serial Port
  if (mySerial.available()) {
    Serial.write((mySerial.read()));
  }

  if (Serial.available()){
    mySerial.write(Serial.read());
  }
}