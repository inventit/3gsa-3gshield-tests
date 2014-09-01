#include <SoftwareSerial.h>
#include <a3gs.h>
#include <sha1.h>

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
    a3gs.write("1024");
    Serial.println("Sent.");
    Sha1.init();
    do {
      int nbytes;
      if ((nbytes = a3gs.read(data, LENGTH)) < 0) {
        Serial.println("read() failed");
        break;
      }
      if (nbytes <= 0) {
        continue;
      }
      Serial.print("Received ");
      Serial.print(nbytes);
      Serial.println(" bytes.");
      for (int i = 0; i < nbytes; i++) {
        Sha1.write(data[i]);
      }
      if (nbytes == LENGTH) {
        continue;
      }
      break;
    } while (true);
    Serial.println("Payload (sha-1):");
    const byte *hash = Sha1.result();
    for (int i = 0; i < 20; i++ ) {
        if (*hash < 0x10) {
          Serial.print("0");
        }
        Serial.print(*hash++, HEX);
    }
    Serial.println();
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

