#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const String ssid = "******";
const String pass = "*****";
const String epURL = "http://d47d-190-165-97-154.ngrok.io/sensor_data";

//float calibration = 22.65; //change this value to calibrate
float calibration = 30.89;
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10], temp;

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

    float ph = readPh();
    if (!isnan(ph)) {
      sendRequest(String(ph));
    }

  } else {
    Serial.println("Error in WiFi connection");
  }
  delay(600000);
}

float readPh() {
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(analogInPin);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)
    avgValue += buf[i];
  float pHVol = (float) avgValue * 5.0 / 1024 / 6;
  float phValue = -5.70 * pHVol + calibration;
  Serial.print("sensor = ");
  Serial.println(phValue);
  return phValue;
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
