// Written in UTF-8, NOT in Windows-31J/Shift_JIS
#include <SoftwareSerial.h>
#include <a3gs.h>

#define SERVER "test.mosquitto.org"
#define PORT 1883

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing.. ");
  const uint8_t payload[] = {
    0x10,0x25,0x00,0x06,'M','Q','I','s','d','p',0x03,0x02,0x00,0x1e,0x00,0x17,
    'e','a','8','c','5','1','3','9','-','c','7','d','8','-','4','c',
    '5','d','-','9','f','6','2'};
  
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
  } else {
    Serial.println("Error.");
    return;
  }

  if (a3gs.connectTCP(SERVER, PORT) != 0) {
    // error
    Serial.println("Connection error.");
    return;
  }
  Serial.print("Data Size:");
  Serial.println(sizeof(payload));
  a3gs.write(payload, sizeof(payload));
  Serial.println("Data Sent.");

  char data[199];
  int c;

  while ((c = a3gs.read(data, 199)) >= 0) {
    Serial.print("Reading ");
    Serial.print(c);
    Serial.println(" bytes:");
    for (int i = 0; i < c; i++) {
      Serial.println(data[i],  HEX);
    }
  }
  Serial.print("c=");
  Serial.println(c);
  Serial.println("Done.");
}

void loop()
{
}
