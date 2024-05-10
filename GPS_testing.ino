#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// The serial connection to the GPS module
SoftwareSerial gpsSerial(4, 5); // RX, TX

TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.println(F("GPS Module test"));
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }

  // If 5000ms pass without any data, something may be wrong.
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo() {
  if (gps.location.isValid()) {
    Serial.print(F("Location: "));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.println(gps.location.lng(), 6);
    Serial.print(F("Altitude: "));
    Serial.print(gps.altitude.meters());
    Serial.println(F("m"));
  } else {
    Serial.println(F("Location: NOT FOUND"));
  }

  if (gps.date.isValid()) {
    Serial.print(F("Date: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.println(gps.date.year());
  }

  if (gps.time.isValid()) {
    Serial.print(F("Time: "));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    Serial.println(gps.time.second());
  }

  Serial.println();
}
