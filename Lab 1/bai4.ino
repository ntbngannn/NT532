int junction1[3]={13,12,11};
int junction2[3]={A0,A1,A2};
int sevenSegment[7]={2,3,4,5,6,7,8};
int indexLed=0;

const byte digitPatterns[] = {
  B11000000,//0 - các thanh từ a-f sáng
  B11111001,//1 - chỉ có 2 thanh b,c sáng
  B10100100,//2
  B10110000,//3
  B10011001,//4
  B10010010,//5
  B10000011,//6
  B11111000,//7
  B10000000,//8
  B10010000,//9
};

void displayNumber(int number) {
  // Check if the number is within the valid range (0-9)
  if (number >= 0 && number <= 9) {
    byte digitPattern = digitPatterns[number];  // Get the digit pattern for the number

    // Set each segment pin based on the corresponding bit in the digit pattern
    for (int i = 0; i < 7; i++) {
      digitalWrite(sevenSegment[i], bitRead(digitPattern, i));
    }
  }
}


void setup() {
  for (int i=2;i<=16;i++){
    if (i!=10 || i!=9 ) pinMode(i, OUTPUT);
  }
}

void loop() {
  for (int i=0;i<18;i++){
    displayNumber((indexLed+=(3*(i==6)+9*(i==9)+6*(i==0)))--);
    digitalWrite(junction1[0], (i<6));
    digitalWrite(junction1[1], (i>=6)*(i<9));
    digitalWrite(junction1[2], i>=9);
    digitalWrite(junction2[0], (i>=9)*(i<15));
    digitalWrite(junction2[1], i>=15);
    digitalWrite(junction2[2], i<9);
    delay(1000);
  }
}
