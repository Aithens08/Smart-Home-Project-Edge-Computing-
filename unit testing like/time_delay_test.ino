#include <SoftwareSerial.h>
#define bluetoothRX 50
#define bluetoothTX 51
SoftwareSerial mySerial(bluetoothRX, bluetoothTX);


unsigned long previousMillis = 0;
const long interval = 60000;

void setup() {
  // put your setup code here, to run once:
    // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Open serial communication for the Bluetooth Serial Port
  mySerial.begin(115200);
}

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