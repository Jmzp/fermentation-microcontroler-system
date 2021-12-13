#include <SoftwareSerial.h>
#include "max6675.h"

int thermoDO = 11;
int thermoCS = 12;
int thermoCLK = 13;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
SoftwareSerial espSerial(5, 6);
double celcius;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  Serial.println("Temperature monitor");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
   celcius = thermocouple.readCelsius();
   Serial.print("C = "); 
   Serial.println(celcius);
   espSerial.println(String(celcius));
   delay(30000);
   //delay(10000);
}
