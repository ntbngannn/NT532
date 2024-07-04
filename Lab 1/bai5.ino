#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C address 0x27, 16 column and 2 rows
int leds[10]={2,3,4,5,6,7,8,9,10,11};
int buttons[3]={A2,A1,A0};
int level=1;
int score=1;


void setup() {
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  for (int i=0;i<10;i++){
    pinMode(leds[i],OUTPUT);
  }
  for (int i=0;i<3;i++)pinMode(buttons[i], INPUT);
  lcd.print("Score:1 Level:1");
}

int displayLed(){
  int total=0;
  for (int i=0;i<10;i++){
    int status=random(0,2);
    total+=status;
    digitalWrite(leds[i],status);
  }
  return total;
}

void offAllLed(){
  for (int i=0;i<10;i++) digitalWrite(leds[i], LOW);
}

void displayLCD(char* text,int col=0,int row=0){
  lcd.setCursor(col, row);
  lcd.print(text);
}

void loop() {
  int num=displayLed()%3;
  unsigned long startTime=millis();
  int result=-1;
  while (millis()-startTime<=4000-level*1000){
    for (int i=0;i<3;i++){
      if (digitalRead(buttons[i])==HIGH){
        result=i;
      }
    }
    if (result!=-1) break;
  }
  lcd.clear();
  char buffer[15];
  if (result==num){
    if (level!=3) level+=1;
    score+=1;
    displayLCD("Correct",0,1);
  }
  else{
    level=1;
    if (score==0) {
      displayLCD("You lose!",0,1);
    } else{
      score--;
      displayLCD("Wrong",0,1);
    }
   
  }
  sprintf(buffer, "Score:%d Level:%d", score,level);
  displayLCD(buffer);
  offAllLed();
  delay(1000);
}
