#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// The serial connection to the GPS module
SoftwareSerial gpsSerial(4, 5); // RX, TX

TinyGPSPlus gps;

#define MPU6050_ADDR 0x68 // MPU6050 I2C address

MPU6050 mpu;
const int vibrationPin = A0; // Vibration sensor's digital output connected to digital pin 2
// const int ledPin = 13;      // Built-in LED on Arduino
const int buz=13;

int16_t gyro_x, gyro_y, gyro_z;

long vibration(){
  delay(10);
  long measurement=pulseIn (vibrationPin, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  gpsSerial.begin(9600);
  Serial.println(F("GPS Module test"));

  pinMode(buz,OUTPUT);
  pinMode(vibrationPin, INPUT);
  // pinMode(ledPin, OUTPUT);

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Wake up MPU6050
  Wire.endTransmission(true);
  
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed. Please check your connection.");
    while (1); // Halt the program if the MPU6050 is not connected properly
  }
}

void loop() {
  // Reading from the vibration sensor
  int vibrationReading = digitalRead(vibrationPin);
  // digitalWrite(ledPin, vibrationReading); // Turn LED on if vibration is detected

  long vibrationDuration = vibration();
  delay(50);
 
  // Print vibration sensor state
  Serial.print("Vibration Duration: ");
  Serial.println(vibrationDuration);

  // Reading from the MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate acceleration in units of g
  float accelX = ax / 16384.0+0.11;
  float accelY = ay / 16384.0+0.11;
  float accelZ = az / 16384.0+0.12;
  float totalAcceleration = sqrt(pow(accelX, 2) + pow(accelY, 2) + pow(accelZ, 2));

  // Calculate gyroscope values in degrees/second
  float gyroX = gx / 131.0;
  float gyroY = gy / 131.0;
  float gyroZ = gz / 131.0;

  // Thresholds
  float accidentAccelThreshold = 2; // Example threshold, adjust based on your needs
  float accidentGyroThreshold = 200.0; // Gyroscope threshold in degrees/sec, adjust as needed
  long vibrationThreshold = 100000; // Vibration duration threshold in microseconds, adjust as needed

  Serial.print(accelX);Serial.print("]");Serial.print(accelY);Serial.print("[");Serial.print(accelZ);Serial.print("(");
  Serial.print(gyroX);Serial.print(")");Serial.print(gyroY);Serial.print("+");Serial.print(gyroZ);Serial.print("-");Serial.print(vibrationDuration);Serial.print("P");
  Serial.println();

  // Print acceleration and gyroscope values
  // Serial.print("Accel X: "); Serial.print(accelX); Serial.print(" g, ");
  // Serial.print("Accel Y: "); Serial.print(accelY); Serial.print(" g, ");
  // Serial.print("Accel Z: "); Serial.print(accelZ); Serial.println(" g");
  // Serial.print("Gyro X: "); Serial.print(gyroX); Serial.print(" deg/s, ");
  // Serial.print("Gyro Y: "); Serial.print(gyroY); Serial.print(" deg/s, ");
  // Serial.print("Gyro Z: "); Serial.print(gyroZ); Serial.println(" deg/s");



  // while (gpsSerial.available() > 0) {
  //   if (gps.encode(gpsSerial.read())) {
  //     displayInfo();
  //   }
  // }

  // // If 5000ms pass without any data, something may be wrong.
  // if (millis() > 5000 && gps.charsProcessed() < 10) {
  //   Serial.println(F("No GPS detected: check wiring."));
  //   while(true);
  // }



  // Detecting accident based on thresholds
  // if (totalAcceleration > accidentAccelThreshold || abs(gyroX) > accidentGyroThreshold || abs(gyroY) > accidentGyroThreshold || abs(gyroZ) > accidentGyroThreshold || vibrationDuration > vibrationThreshold) {
  //   Serial.println("Accident Detected!");
  //   digitalWrite(buz,HIGH);
  //   delay(10000);
  //   digitalWrite(buz, LOW);
  //   if (gpsSerial.available() > 0) 
  //   {
  //   // Read the data from GPS module
  //   char gpsData = gpsSerial.read();
  //   // Print the received data to the Serial Monitor
  //   Serial.print(gpsData);
  //   }
  //   // Here you could add code to take further actions, like sending an alert message through GSM
  //   while(1); // Stop the loop; remove or modify as needed for your application
  // }

  delay(500); // Delay for readability
}


void displayInfo() 
{
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