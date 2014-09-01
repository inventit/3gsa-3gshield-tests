#include <SoftwareSerial.h>
#include <a3gs.h>
#include <sha1.h>

const char *server = "your-test-server-address";
const int port = 8001;

#define LENGTH 256
byte data[LENGTH];
#define c 0x0a

void setup()
{
  Serial.begin(9600);

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    if (a3gs.connectTCP(server, port) != 0) {
      Serial.println("connectTCP() can't connect");
      return; 
    }
    Sha1.init();
    for (int i = 0; i < LENGTH; i++ ) {
      data[i] = c;
      Sha1.write(data[i]);
    }
    const int wbytes = a3gs.write(data, LENGTH);
    Serial.print("Sent ");
    Serial.print(wbytes);
    Serial.println(" bytes.");
    Serial.println("Payload (sha-1):");
    const byte *hash = Sha1.result();
    for (int i = 0; i < 20; i++ ) {
        if (*hash < 0x10) {
          Serial.print("0");
        }
        Serial.print(*hash++, HEX);
    }
    Serial.println();
    delay(3000);
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

