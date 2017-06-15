#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define DHTTYPE DHT11  // may need to change depending on type of DHT sensor
#define DHTPIN  5      // may need to change this pin depending on ESP8266 - find pin layout for your board

DHT dht(DHTPIN, DHTTYPE, 11);
HTTPClient http;
const char* wifiName = "********";
const char* password = "********";
int humidity, temp;
const char* host = "*********";

void setup(void) {
  connect_to_wifi();
}

void loop(void) {
  if (WiFi.status()== WL_CONNECTED) {

    get_temperature();
    send_data_request();
    delay(60000); // send request every minute

  } else {
    Serial.println("Error in WiFi connection");
  }

}

void connect_to_wifi() {
  Serial.begin(115200);
  dht.begin();

  // Connect to WiFi network
  WiFi.begin(wifiName, password);
  Serial.print("\n\r \n\rTrying to connect");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting...");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void get_temperature() {
    // Reading temperature
    humidity = (int) dht.readHumidity();          // Read humidity percentage
    temp = (int) dht.readTemperature(true);     // Read temperature in C
    // Check for failed reads
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("DHT Sensor failed to read.");
      return;
    }
 }

void send_data_request() {
  Serial.printf("\n[Connecting to %s ... ", host);
  http.begin(host);

  http.addHeader("Accept", "application/json");
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(encode_json());
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);

  http.end();

}

String encode_json() {
  StaticJsonBuffer<200> jsonBuffer;
  String json;
  JsonObject& dataObject = jsonBuffer.createObject();
  dataObject.set("temperature", temp);
  dataObject.set("humidity", humidity);
  dataObject.printTo(json);
  return json;
}
