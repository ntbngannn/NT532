#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <BH1750.h>

#define WIFI_AP_NAME        "MSI5958"
#define WIFI_PASSWORD       "12345678"

#define THINGSBOARD_SERVER  "23.22.141.33"
#define THINGSBOARD_PORT    1883

#define DHTPIN 16
#define DHTTYPE DHT22

BH1750 lightMeter;
const size_t JSON_BUFFER_SIZE = JSON_OBJECT_SIZE(1) + 20;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi network with IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(THINGSBOARD_SERVER, THINGSBOARD_PORT);
  client.setCallback(callback);
  client.setKeepAlive(60);
  Wire.begin(21,22);
  dht.begin();
  lightMeter.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float light = lightMeter.readLightLevel();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.println("test");
  publishTelemetry("temperature", temperature);
  publishTelemetry("humidity", humidity);
  publishTelemetry("light", light);
  delay(1000); 
}

void publishTelemetry(const char* key, float value) {
  StaticJsonDocument<JSON_BUFFER_SIZE> jsonBuffer;
  jsonBuffer[key] = value;
  
  String jsonPayload;
  serializeJson(jsonBuffer, jsonPayload);
  
  client.publish("v1/devices/me/telemetry", jsonPayload.c_str());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("esp_client","nhom8","123456")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
    }
  }
}