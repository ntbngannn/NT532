const int PIR = 13;

void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);
}

void loop() {
  int value = digitalRead(PIR);
  if (value == 0)
  {
    Serial.println("Không phát hiện chuyển động");
  }
  else Serial.println("Phát hiện chuyển động!!!");
  delay(1000);
}
