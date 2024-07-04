#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#define Led1 D6
#define Led2 D7

const char* ssid="MSI5958";
const char* password_wifi="12345678";
const char* mqtt_server = "23.22.141.33";
const int mqttPort = 1883;
const char* mqttUser = "nhom208";
const char* mqttPassword = "123456";
const char* client_id="wemos_client";

WiFiClient espClient;
PubSubClient client(espClient);

void initWifi(){
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  WiFi.begin( ssid, password_wifi, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected !");
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  StaticJsonDocument<50> jsonBuffer;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  DeserializationError error = deserializeJson(jsonBuffer,  messageTemp);
  if (error) {
    Serial.print("Lỗi khi phân tích cú pháp JSON: ");
    Serial.println(error.c_str());
    return; // Thoát ra nếu có lỗi
  }
  String method=jsonBuffer["method"];
  bool value=jsonBuffer["params"];
  if (method=="setValueLed1"){
    digitalWrite(Led1,value);
  }
  else digitalWrite(Led2,value);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  initWifi();
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback); 
}

void connectMQTT(){
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect(client_id, mqttUser, mqttPassword )) {
      Serial.println("Mqtt connected");  
      client.subscribe("v1/devices/me/rpc/request/+");     
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void loop() {
  connectMQTT();
  client.loop();
  delay(1000);
}