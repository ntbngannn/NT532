#include <DHT.h> 
 
const int DHTPIN = 8; 
const int DHTTYPE = DHT22; 
 
DHT dht(DHTPIN, DHTTYPE);

const int ledPin1 = 5; 
const int ledPin2 = 6; 
const int ledPin3 = 7;  

void setup() {
  Serial.begin(9600);
  pinMode(DHTPIN,INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();
  if (t<=25) //Nhiệt độ cao mức 1
  {        
    if (h>70) //độ ẩm vượt ngưỡng thì tắt hết quạt
    {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      Serial.println("Temperature: " + String(t) + "°C");
      Serial.println("Humidity: " + String(h) + "%");
    }
    else {
    digitalWrite(ledPin1, HIGH);
    Serial.println("Temperature: " + String(t) + "°C");
    Serial.println("Humidity: " + String(h) + "%");}
  } 
  else if (t>25 && t<=26) //Nhiệt độ cao mức 2
  {
    if (h>70) //độ ẩm vượt ngưỡng thì tắt hết quạt
    {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      Serial.println("Temperature: " + String(t) + "°C");
      Serial.println("Humidity: " + String(h) + "%");
    }
    else { 
      digitalWrite(ledPin1, HIGH);
      digitalWrite(ledPin2, HIGH);
      Serial.println("Temperature: " + String(t) + "°C");
      Serial.println("Humidity: " + String(h) + "%");
    }
  }
  else if (t>26) //Nhiệt độ cao nhất
  {
    Serial.println("Temperature: " + String(t) + "°C");
    Serial.println("Humidity: " + String(h) + "%");
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
  }  
  else {
    Serial.println("Temperature: " + String(t) + "°C");
    Serial.println("Humidity: " + String(h) + "%");
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
  }
  delay(1000);
}
