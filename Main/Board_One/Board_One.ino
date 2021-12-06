#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Ultrasonic Setting
#define echoPin_Front D2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin_Front D1 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPin_Back D4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin_Back D3
#define echoPin_Left D6
#define trigPin_Left D5 
#define echoPin_Right D8 
#define trigPin_Right D7
#define Buzzer_pin D9

//PIR Setting
int PIR_pin_front = A1;
int PIR_pin_back = A2;
int PIR_pin_left = A3;
int PIR_pin_right = A4;
int PIR_front_state = LOW;
int PIR_back_state = LOW;
int PIR_left_state = LOW;
int PIR_right_state = LOW;
int calibrationTime = 60;


//LED_Setting
#define LED_PIN A0
#define LED_COUNT 32
int pixelInterval = 50;
uint16_t pixelNumber = LED_COUNT;
uint16_t      pixelCurrent = 0;
Adafruit_NeoPixel strip(pixelNumber, LED_PIN, NEO_GRB + NEO_KHZ800);


// Buzzer
bool too_close = false;
bool motion_detected = false;

void setup(){
  Serial.begin(115200);

  // Ultrasonic
  pinMode(trigPin_Front, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin_Front, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin_Back, OUTPUT);
  pinMode(echoPin_Back, INPUT);
  pinMode(trigPin_Left, OUTPUT);
  pinMode(echoPin_Left, INPUT);
  pinMode(trigPin_Right, OUTPUT);
  pinMode(echoPin_Right, INPUT);
  pinMode(Buzzer_pin, OUTPUT);


  // PIR
  pinMode(PIR_pin_front, INPUT);
  pinMode(PIR_pin_back, INPUT);
  pinMode(PIR_pin_left, INPUT);
  pinMode(PIR_pin_right, INPUT);
  // Calibrate PIR sensor
  Serial.print("Calibrating Sensor ");
  for(int i = 0; i < calibrationTime; i++){
     Serial.print(".");
     delay(1000);
  }
  Serial.println(" done.");
  Serial.println("SENSOR ACTIVE");
  delay(50);


  //LED
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50);


  // Buzzer
  pinMode(Buzzer_pin, OUTPUT);
}


void loop(){
  colorWipe(strip.Color(0, 255, 0), 50);

  PIR_Front();
  Ultrasonic_Front();
  if(too_close == true && motion_detected == true){
    colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;
  motion_detected = false;
  PIR_Back();
  Ultrasonic_Back();
  if(too_close == true && motion_detected == true){
    colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;
  motion_detected = false;
  PIR_Left();
  Ultrasonic_Left();
  if(too_close == true && motion_detected == true){
    colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;
  motion_detected = false;
  PIR_Right();
  Ultrasonic_Right();
  if(too_close == true && motion_detected == true){
    colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;
  motion_detected = false;
}



/**************************************** Ultrasonic **************************************/
void Ultrasonic_Front(){
  delay(50);
  digitalWrite(trigPin_Front, LOW);
  delay(500);
  digitalWrite(trigPin_Front, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_Front, LOW);
  long duration = pulseIn(echoPin_Front, HIGH);
  int distance = duration * 0.034 / 2;
  if(distance <= 100){
       too_close = true;
  }
  Serial.print("Distance of front sensor: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void Ultrasonic_Back(){
  delay(50);
  digitalWrite(trigPin_Back, LOW);
  delay(500);
  digitalWrite(trigPin_Back, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_Back, LOW);
  long duration = pulseIn(echoPin_Back, HIGH);
  int distance = duration * 0.034 / 2;
  if(distance <= 100){
       too_close = true;
  }
  Serial.print("Distance of back sensor: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void Ultrasonic_Left(){
  delay(50);
  digitalWrite(trigPin_Left, LOW);
  delay(500);
  digitalWrite(trigPin_Left, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_Left, LOW);
  long duration = pulseIn(echoPin_Left, HIGH);
  int distance = duration * 0.034 / 2;
  if(distance <= 100){
       too_close = true;
  }
  Serial.print("Distance of left sensor: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void Ultrasonic_Right(){
  delay(50);
  digitalWrite(trigPin_Right, LOW);
  delay(500);
  digitalWrite(trigPin_Right, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_Right, LOW);
  long duration = pulseIn(echoPin_Right, HIGH);
  int distance = duration * 0.034 / 2;
  if(distance <= 100){
       too_close = true;
  }
  Serial.print("Distance of right sensor: ");
  Serial.print(distance);
  Serial.println(" cm");
}


/*********************************** PIR **********************************/
void PIR_Front(){
  PIR_front_state = digitalRead(PIR_pin_front);
  if(PIR_front_state == HIGH){
      Serial.println("Front Detected!");
      motion_detected = true;
  }
  PIR_front_state = LOW;
}

void PIR_Back(){
  PIR_back_state = digitalRead(PIR_pin_back);
  if(PIR_back_state == HIGH){
      Serial.println("Back Detected!");
      motion_detected = true;
  }
  PIR_back_state = LOW;
}

void PIR_Left(){
  PIR_left_state = digitalRead(PIR_pin_left);
  if(PIR_left_state == HIGH){
      Serial.println("Left Detected!");
      motion_detected = true;
  }
  PIR_left_state = LOW;
}

void PIR_Right(){
  PIR_right_state = digitalRead(PIR_pin_right);
  if(PIR_right_state == HIGH){
      Serial.println("Right Detected!");
      motion_detected = true;
  }
  PIR_right_state = LOW;
}


/************************************* LED *********************************************/
void colorWipe(uint32_t color, int wait) {
  for(int i = 0; i < pixelNumber; i++){
    if(pixelInterval != wait){
      pixelInterval = wait;
     }
    strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
    strip.show();  
  }
}

void colorWipe_Red(uint32_t color, int wait) {
  for(int i = 0; i < pixelNumber; i++){
    if(pixelInterval != wait){
      pixelInterval = wait;
     }
    strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
    strip.show();
    tone(Buzzer_pin, 1000);
      
  }
  delay(500);
}
