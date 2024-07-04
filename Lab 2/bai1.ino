#define trigPin 8
#define echoPin 7
const int warningLed = 11;
const String warningStatus;
void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(warningLed, OUTPUT);
}
void blink(int distance){
  if (distance > 0 && distance <= 10)
  {
    digitalWrite(warningLed, HIGH);
    delay(1);
    digitalWrite(warningLed, LOW);
    warningStatus = "Mức cảnh báo: 5";
  }
  else if (distance > 10 && distance <= 20)
  {
    digitalWrite(warningLed, HIGH);
    delay(100);
    digitalWrite(warningLed, LOW);
    warningStatus = "Mức cảnh báo: 4";
  }
    else if (distance > 20 && distance <= 30)
  {
    digitalWrite(warningLed, HIGH);
    delay(200);
    digitalWrite(warningLed, LOW);
    warningStatus = "Mức cảnh báo: 3";
  }
    else if (distance > 30 && distance <= 40)
  {
    digitalWrite(warningLed, HIGH);
    delay(300);
    digitalWrite(warningLed, LOW);
    warningStatus = "Mức cảnh báo: 2";
  }
    else if (distance > 40 && distance <= 50)
  {
    digitalWrite(warningLed, HIGH);
    delay(400);
    digitalWrite(warningLed, LOW);
    warningStatus = "Mức cảnh báo: 1";
  }
  else
  {
    warningStatus = "Ngoài phạm vi";
  }
}
void loop() 
{
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance <= 100 && distance > 0)
  {
    blink(distance);
    Serial.println("Khoảng cách: " + String(distance) +"cm - " + String(warningStatus));
  }  
  delay(500);
}