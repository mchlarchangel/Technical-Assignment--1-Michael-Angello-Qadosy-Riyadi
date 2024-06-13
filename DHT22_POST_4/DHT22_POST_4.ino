
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <ArduinoJson.h>
#define DHT22PIN 4


DHT dht(DHT22PIN, DHT22);

const char* ssid = "Redmi Note 10S";
const char* password = "daffa1258";
String payload;
const String url = "http://192.168.39.108:5000/sensors";

void setup() {
  dht.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected");

  Serial.println("HTTP Started");
}

void loop() {
  float y = dht.readHumidity();
  float x = dht.readTemperature();

    HTTPClient http;
    http.begin(url);

  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
  doc["temperature"] = x;
  doc["humidity"] = y;
  doc["time"] = millis();
  String requestBody;
  serializeJson(doc, requestBody);

  int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();

  delay(2000);
}

