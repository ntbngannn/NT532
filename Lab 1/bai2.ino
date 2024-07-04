const int ledPin1 = 13; 
const int ledPin2 = 12; 
const int ledPin3 = 11;  
const int potPin = A0;  

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin);  
  int delayTime;
  String speedState;
  
  // Khoảng giá trị biến trở từ 0-340: Tốc độ chậm
  if (potValue < 341) {  
    delayTime = 1000;  
    speedState = "Chậm";
  }
  // Khoảng giá trị biến trở từ 341-681: Tốc độ trung bình
   else if (potValue < 682) {  
    delayTime = 500;  
    speedState = "Trung bình";
  } 
  // Khoảng giá trị biến trở từ 682-1023: Tốc độ nhanh
  else {  
    delayTime = 200; 
    speedState = "Nhanh";
  }
  for  (int i=11;i<=13; i++){

  // Điều chỉnh tốc độ đèn LED, đèn trước tắt -> đèn sau mới sáng
    digitalWrite(i, HIGH);
    delay(delayTime);
    digitalWrite(i, LOW);
  }

  Serial.print("Tốc độ: ");
  Serial.println(speedState + " (" + potValue + ")");

  delay(50);  
}