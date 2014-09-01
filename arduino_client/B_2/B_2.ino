#include <SoftwareSerial.h>
#include <a3gs.h>
#include <sha1.h>

const char *server = "your-test-server-address";
const int port = 8001;

#define LENGTH 10000
#define BUFF   200
byte data[BUFF];

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
    int x = 0;
    int j = 0;
    int max = 0;
    for (int i = 0; i < ((LENGTH - 1) / BUFF + 1); i++ ) {
      Sha1.init();
      max = BUFF * i + BUFF;
      if (max > LENGTH) {
        max = LENGTH;
      }
      data[0] = 0x00;
      Sha1.write(data[0]);
      x++;
      for (j = 1; x < max; x++) {
        data[j] = x % 256;
        Sha1.write(data[j]);
        j++;
      }
      const int wbytes = a3gs.write(data, j);
      Serial.print("Sent ");
      Serial.print(wbytes);
      Serial.println(" bytes.");
      Serial.println("Payload (sha-1):");
      const byte *hash = Sha1.result();
      for (j = 0; j < 20; j++ ) {
        if (*hash < 0x10) {
          Serial.print("0");
        }
        Serial.print(*hash++, HEX);
      }
      Serial.println();
    }
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

