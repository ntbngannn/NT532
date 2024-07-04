
const int button = 13;
const int ledCount = 12;
int ledPins[ledCount] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int ledIndex = 0;

void setup() {
  pinMode(0, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  for (int i = 0; i < ledCount; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void updateLEDs() {
  for (int i = 0; i < ledCount; i++) {
    if (i < ledIndex) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
} 

void loop() {
    if (digitalRead(button) == LOW) {
      ledIndex++;
      if (ledIndex > ledCount) {
        ledIndex = 1;
      }
      updateLEDs();
      delay(200);
    }
}
