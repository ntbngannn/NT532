#include <BH1750.h>

BH1750 lightMeter;

int mode=10;
int button=3;
int Leds[10]={4,5,6,7,8,9,10,11,12,13};

int previousButtonState = 0; // Trạng thái trước đó của nút bấm
int buttonState = 0; // Trạng thái nút bấm
int numPress=0;
int timerBegin;
int timerEnd;

void setup() {
  pinMode(button, INPUT);
  lightMeter.begin();
  Serial.begin(9600);
  for (int i=0;i<10;i++) pinMode(Leds[i], OUTPUT);
  Serial.println(F("BH1750 Test begin"));
}

void displayLed(int count){
  for (int i=0;i<10;i++) {
    if (i<count) digitalWrite(Leds[i],1);
    else digitalWrite(Leds[i],0);
  }
}

int readMode(){
  int result;
  buttonState=digitalRead(button);
  delay(50);
  if (buttonState==1 && previousButtonState==0){
    numPress++;
    if (numPress==1) timerBegin=millis();
    else{
      numPress=0;
      result=10;
    }
  } else result=mode;
  previousButtonState=buttonState;
  return result;
}

void loop() {
  mode=readMode();
  timerEnd=millis();
  if (numPress==1 && timerEnd-timerBegin>500){
    mode=5;
    numPress=0;
  }
  
  float lux = lightMeter.readLightLevel();
  Serial.println(lux);
  int value=map(lux,0,5000,mode,0);
  displayLed(value);
}
