#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const String ssid = "*****";
const String pass = "*****";
const String epURL = "http://d47d-190-165-97-154.ngrok.io/sensor_data";

void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, pass); //WiFi connection

  while (WiFi.status() != WL_CONNECTED) { //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP()); // Print the IP address

}

void loop() {

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    WiFiClient client;
    HTTPClient http;
        
     if(Serial.available() > 0)
     {
        String str = Serial.readStringUntil('\n');
        Serial.println(str);
        str.trim();
        if (!isnan(str.toFloat())){
          sendRequest(str);
        }
     }

  } else {

    Serial.println("Error in WiFi connection");

  }
  //delay(5000);
  delay(30000);
}

void sendRequest(String value) {
  WiFiClient client;
  HTTPClient http;
  DynamicJsonDocument doc(200);
  String json;

  http.begin(client, epURL); //Specify request destination
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  doc["value"] = value;
  doc["sensor_mac"] = WiFi.macAddress();
  serializeJson(doc, json);
  Serial.println(json);
  int httpCode = http.POST(json); //Send the request
  String payload = http.getString(); //Get the response payload
  Serial.println(httpCode); //Print HTTP return code
  Serial.println(payload); //Print request response payload
  http.end(); //Close connection
}
