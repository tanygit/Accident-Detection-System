#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// ThingSpeak API URL and API Key
String URL = "http://api.thingspeak.com/update?api_key=LH1NPIBZH5A2Z0LW";

// http://api.thingspeak.com/update?api_key=JKX5F7ECNCC2REAX&field


int i=1;

void setup() {
  Serial.begin(9600);
  
  // Connect to WiFi network
  WiFi.begin("Kori", "dvdu0231"); 
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("WiFi Connected");
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    int ax1 = data.indexOf("]");
    int ay1 = data.indexOf("[");
    int az1 = data.indexOf("(");
    int gx1 = data.indexOf(")");
    int gy1 = data.indexOf("+");
    int gz1 = data.indexOf("-");
    int vib1 = data.indexOf("P");
 
    
    if (ax1 != -1 && ay1 != -1 && az1 != -1 && gx1 != -1 && gy1 != -1 && gz1 != -1) {
      float ax = data.substring(0, ax1).toFloat();
      float ay = data.substring(ax1 + 1, ay1).toFloat();
      float az = data.substring(ay1 + 1, az1).toFloat();
      float gx = data.substring(az1 + 1, gx1).toFloat();
      float gy = data.substring(gx1 + 1, gy1).toFloat();
      float gz = data.substring(gy1 + 1, gz1).toFloat();
      float vib = data.substring(gz1 + 1, vib1).toFloat();
      
      // Print sensor data to Serial monitor
      Serial.print("Accel X: "); Serial.print(ax); Serial.print(" g, ");
      Serial.print("Accel Y: "); Serial.print(ay); Serial.print(" g, ");
      Serial.print("Accel Z: "); Serial.print(az); Serial.println(" g");
      Serial.print("Gyro X: "); Serial.print(gx); Serial.print(" deg/s, ");
      Serial.print("Gyro Y: "); Serial.print(gy); Serial.print(" deg/s, ");
      Serial.print("Gyro Z: "); Serial.print(gz); Serial.println(" deg/s");
      Serial.print("Vibartion reading: "); Serial.print(vib); Serial.println(" s");
   
      // Send sensor data to ThingSpeak
      sendData(ax, ay, az, gx, gy, gz ,vib);
    }
  }
}

void sendData(float ax, float ay, float az, float gx, float gy, float gz, float vib) {
  WiFiClient client;
  HTTPClient http;
  
  // Construct the URL with query parameters
  String dataUrl = URL + "&field1=" + String(ax) +
                             "&field2=" + String(ay) +
                             "&field3=" + String(az) +
                             "&field4=" + String(gx) +
                             "&field5=" + String(gy) +
                             "&field6=" + String(gz) +
                             "&field7=" + String(vib);

  
  Serial.print("Sending data to ThingSpeak... ");
  
  http.begin(client, dataUrl); // Begin HTTP connection with WiFiClient object and URL
  
  int httpResponseCode = http.GET(); // Send the GET request
  
  if (httpResponseCode > 0) {
    String response = http.getString(); // Get the response payload
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end(); // Close HTTP connection
}







