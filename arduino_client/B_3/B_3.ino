#include <a3gs.h>
#include <SoftwareSerial.h>
//#include <sha1.h>

const char *server = "your-test-server-address";
const int port = 8001;
#define LENGTH 256
byte data[LENGTH];

void setup()
{
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    if (a3gs.connectTCP(server, port) != 0) {
      Serial.println("connectTCP() can't connect");
      return;
    }
    a3gs.write("512");
    Serial.println("Sent.");
    do {
      int nbytes;
      if ((nbytes = a3gs.read(data, LENGTH)) < 0) {
        Serial.println("read() failed");
        break;
      }
      if (nbytes <= 0) {
        continue;
      }
      Sha1.init();
      Serial.print("Received ");
      Serial.print(nbytes);
      Serial.println(" bytes.");
      Serial.println("Payload (raw):");
      for (int i = 0; i < nbytes; i++) {
          if (data[i] < 0x10) {
            Serial.print("0");
          }
          Serial.print(data[i], HEX);
          Sha1.write(data[i]);
      }
     const byte *hash = Sha1.result();
     for (int i = 0; i < 20; i++ ) {
         if (*hash < 0x10) {
           Serial.print("0");
         }
         Serial.print(*hash++, HEX);
     }
      Serial.println();
      break;
    } while (true);
  } else {
    Serial.println("Failed.");
  }
  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();

}

void loop()
{
}

