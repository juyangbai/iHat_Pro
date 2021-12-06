const int buzzerPin = A0;
boolean flag = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag == false){
    tone(buzzerPin, 0);
    delay(5000);
    flag = true;
  } 
  else{
    tone(buzzerPin, 1000);
    delay(5000);
    flag = false;
  }
}
