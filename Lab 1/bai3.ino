int button=12;
int led[6]={11,10,9,8,7,6};
void setup() {
  pinMode(13, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(13, HIGH);
}

void Dice(){
  int rand=random(1,6);
  for (int i=0;i<rand;i++){
    digitalWrite(led[i], HIGH);
  }
  delay(1000);
  for (int i=0;i<rand;i++){
    digitalWrite(led[i],LOW);
  }
  Blink(rand);
}

void Blink(int num){
  for (int i=0;i<7;i++){
    for (int j=0;j<num;j++){
      digitalWrite(led[j],1);
    }
    delay(200);
    for (int j=0;j<num;j++){
      digitalWrite(led[j],0);
    }
    delay(200);
  }
}  

void start(){
  for (int i=0;i<6;i++){
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i],1);
    delay(200);
    digitalWrite(led[i],0);
  }
  for (int i=5 ;i>=0;i--){
    pinMode(led[i], OUTPUT);
    digitalWrite(led[i],1);
    delay(200);
    digitalWrite(led[i],0);
  }
}

void loop() {
  if  (digitalRead(button)==HIGH){
    start();
    Dice();
  }
}
