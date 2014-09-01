#include "Arduino.h"

#include <SoftwareSerial.h>
#include <a3gs.h>
#include <sha1.h>

const char *server = "your-test-server-address";
const int port = 8001;
#define LENGTH 321
byte data[LENGTH];

void setup() {
  Serial.begin(9600);
  delay(3000);  // Wait for Start Serial Monitor

  Serial.print("Initializing.. ");
  if (a3gs.start() == 0 && a3gs.begin() == 0) {
    Serial.println("Succeeded.");
    if (a3gs.connectTCP(server, port) != 0) {
      Serial.println("connectTCP() can't connect");
      return;
    }
}

boolean isValidContent(const byte *payload, const int size) {
    // The first 300 bytes for data, the remainder 20bytes for SHA-1 checksum
    if (size != 320) {
        return false;
    }

    Sha1.init();
    for (int i = 0; i < 300; i++) {
        Sha1.write(*(payload + i));
    }
    const byte *hash = Sha1.result();
    boolean ok = true;
    for (int i = 0; i < 20; i++ ) {
        if (*(payload + 300 + i) != *(hash + i)) {
            Serial.println("Broken payload..");
            ok = false;
            break;
        }
    }
    return ok;
}

int prepareRequest() {
    int length = 0;
    data[length++] = 0x02;
    Sha1.init();
    for (int i = 0; i < 300; i++) {
        data[length++] = millis() % 256;
    }
    const byte *hash = Sha1.result();
    for (int i = 0; i < 20; i++ ) {
        data[length++] = *(hash + i);
    }
    return length;
}

void loop() {
    const int length = prepareRequest();
    a3gs.write(data, length);
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
      Serial.print("Received ");
      Serial.print(nbytes);
      Serial.println(" bytes.");
      break;
    } while (true);
    Serial.println("Payload (sha-1):");
    if ()
    Serial.println();
  } else {
    Serial.println("Failed.");
  }
  Serial.println("Shutdown..");
  a3gs.end();
  a3gs.shutdown();
}

