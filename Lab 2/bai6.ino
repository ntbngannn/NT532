const int PIR = 13;
const int ledPin1 = 12;
const int ledPin2 = 11;

void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void blink() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  delay(100);
  digitalWrite(ledPin1, LOW);
  delay(100);
  digitalWrite(ledPin2, HIGH);
  delay(50);
  digitalWrite(ledPin2, LOW);
}

void loop() {
  int value = digitalRead(PIR);
  if (value == 1) {
      Serial.println("Phát hiện chuyển động");
      blink();
    }
  else {
    Serial.println("Không phát hiện chuyển động!!!");
  }
  delay(1000);
}