#define trigPin 10
#define echoPin 9
int sevenSegment[7]={2,3,4,5,6,7,8};

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  for (int i=0;i<7;i++) pinMode(sevenSegment[i], OUTPUT);
}

const byte digitPatterns[] = {
  B11000000,//0 
  B11111001,//1 
  B10100100,//2
  B10110000,//3
  B10011001,//4
  B10010010,//5
  B10000010,//6
  B11111000,//7
  B10000000,//8
  B10010000,//9'
  B11111111,//off
};

void displayNumber(int number) {
  if (number >= 0 && number <= 10) {
    byte digitPattern = digitPatterns[number]; 
    for (int i = 0; i < 7; i++) {
      digitalWrite(sevenSegment[i], bitRead(digitPattern, i));
    }
  }
}

long readDistance(){
  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void BlinkZero(){
  displayNumber(0);
  delay(300);
  displayNumber(10);
}


void loop() {
  long distance=readDistance();
  if (distance>100 || distance<=0) BlinkZero();
  else {
    int num=map(distance,1,100,9,1);
    displayNumber(num);
  }
  delay(500);
}
