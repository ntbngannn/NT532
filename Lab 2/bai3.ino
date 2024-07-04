int Led=13;
int MQ135=A0;


void setup() {
  pinMode(Led, OUTPUT);
  pinMode(MQ135, INPUT);
  Serial.begin(9600);
}

void Blink(int speed){

  digitalWrite(Led,1);
  delay(speed);
  digitalWrite(Led,0);
  delay(speed);
}

void loop() {
  int value=analogRead(MQ135);
  if (value<=200) 
  {
    Blink(800);
    Serial.println(value);
    Serial.println("Chậm");
  }
  else if (value<=700) 
  {
    Blink(500);
    Serial.println(value);
    Serial.println("Trung bình");
  }
  else 
  {
    Blink(200);
    Serial.println(value);
    Serial.println("Nhanh");
  }
}