#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial BTSerial(50, 51); // RX | TX

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(115200);  
}

void loop()
{
  if (BTSerial.available())
  {
    String receivedString = BTSerial.readString();
    Serial.println(receivedString);
  }

  if (Serial.available())
  {
    String sendString = Serial.readString();

    // create a new JSON object
    StaticJsonDocument<200> doc;
    doc["hour"] = 9;
    doc["minute"] = 42;
    doc["second"] = 15;

    // serialize the JSON object to a string
    String jsonString;
    serializeJson(doc, jsonString);

    // send the JSON string to the Bluetooth module
    BTSerial.println(jsonString);
  }
}