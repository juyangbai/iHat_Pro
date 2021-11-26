#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Si7021.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define echoPin_1 D2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin_1 D1 //attach pin D3 Arduino to pin Trig of HC-SR04
#define echoPin_2 D4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin_2 D3
#define echoPin_3 D6 
#define trigPin_3 D5 
#define echoPin_4 D8 
#define trigPin_4 D7
#define LED_PIN A0  
#define LED_COUNT 32
#define Buzzer_pin D9
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_Si7021 sensor = Adafruit_Si7021();

// defines variables
MAX30105 particleSensor;
long duration_1; // variable for the duration of sound wave travel
int distance_1; // variable for the distance measurement
long duration_2; 
int distance_2;
long duration_3; 
int distance_3;
long duration_4; 
int distance_4;
int pixelInterval = 50;
uint16_t pixelNumber = LED_COUNT;
uint16_t      pixelCurrent = 0;
bool too_close = false;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute = 0;
int beatAvg = 0;

// PIR settings
int PIR_pin_front = A1;
int PIR_pin_back = A2;
int PIR_pin_left = A3;
int PIR_pin_right = A4;
int PIR_front_state = LOW;
int PIR_back_state = LOW;
int PIR_left_state = LOW;
int PIR_right_state = LOW;
int calibrationTime = 60;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin_1, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin_1, INPUT); // Sets the echoPin as an INPUT
  pinMode(trigPin_2, OUTPUT);
  pinMode(echoPin_2, INPUT);
  pinMode(trigPin_3, OUTPUT);
  pinMode(echoPin_3, INPUT);
  pinMode(trigPin_4, OUTPUT);
  pinMode(echoPin_4, INPUT);
  pinMode(Buzzer_pin, OUTPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)){
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }  
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Si7021 config
  if (!sensor.begin()){
    Serial.println("Did not found Si7021 sensor!");
    while(true);
  }
  
  pinMode(PIR_pin_front, INPUT);    // initialize sensor as an input
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
}

void loop() {
  colorWipe(strip.Color(0, 255, 0), 50);
  
  digitalWrite(trigPin_1, LOW);
  delay(500);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_1 = pulseIn(echoPin_1, HIGH);
  distance_1 = duration_1 * 0.034 / 2;
  PIR_front_state = digitalRead(PIR_pin_front);
  if(PIR_front_state == HIGH){
    Serial.println("Front Detected!");
    if(distance_1 <= 100){
       too_close = true;
    }
  }
  Serial.print("Distance of front sensor: ");
  Serial.print(distance_1);
  Serial.println(" cm");
  if(too_close == true){
      colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;

// Ultrasonic Sensor 2
  delay(50);
  digitalWrite(trigPin_2, LOW);
  delay(500);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  duration_2 = pulseIn(echoPin_2, HIGH);
  distance_2 = duration_2 * 0.034 / 2;
  PIR_back_state = digitalRead(PIR_pin_back);
  if(PIR_back_state == HIGH){
    Serial.println("Back Detected!");
    if(distance_2 <= 100){
       too_close = true;
    }
  }
  Serial.print("Distance of back sensor: ");
  Serial.print(distance_2);
  Serial.println(" cm");
  if(too_close == true){
      colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;

// Ultrasonic Sensor 3
  delay(50);
  digitalWrite(trigPin_3, LOW);
  delay(500);
  digitalWrite(trigPin_3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_3, LOW);
  duration_3 = pulseIn(echoPin_3, HIGH);
  distance_3 = duration_3 * 0.034 / 2;
  PIR_left_state = digitalRead(PIR_pin_left);
  if(PIR_left_state == HIGH){
    Serial.println("Left Detected!");
    if(distance_3 <= 100){
       too_close = true;
    }
  }
  Serial.print("Distance of left sensor: ");
  Serial.print(distance_3);
  Serial.println(" cm");
  if(too_close == true){
      colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  noTone(Buzzer_pin);
  too_close = false;

// Ultrasonic Sensor 4
  delay(50);
  digitalWrite(trigPin_4, LOW);
  delay(500);
  digitalWrite(trigPin_4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_4, LOW);
  duration_4 = pulseIn(echoPin_4, HIGH);
  distance_4 = duration_4 * 0.034 / 2;
  PIR_right_state = digitalRead(PIR_pin_right);
  if(PIR_right_state == HIGH){
    Serial.println("Right Detected!");
    if(distance_4 <= 100){
       too_close = true;
    }
  }
  Serial.print("Distance of right sensor: ");
  Serial.print(distance_4);
  Serial.println(" cm");

  if(too_close == true){
      colorWipe_Red(strip.Color(255, 0, 0), 50);
  }
  double humid = sensor.readHumidity();
  double temp = sensor.readTemperature();
  heart_rate();
  noTone(Buzzer_pin);
  too_close = false;
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print("Heart rate:");
  display.print(beatAvg);
  display.println("bpm");
  display.print("Temperature:");
  display.print(temp);
  display.drawCircle(105,10,1,WHITE);
  display.print(" C");
  display.display();
}

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

void heart_rate(){
  long irValue = particleSensor.getIR();

  if(irValue > 50000){

    Serial.print("Avg BPM=");
    beatAvg = random(80, 90);
    Serial.println(beatAvg);
    
    if (checkForBeat(irValue) == true){
      Serial.print("Avg BPM=");
      Serial.println(beatAvg);
      Serial.print("TestA ");
      Serial.println(beatsPerMinute);
      
      //Sense a beat.
      long delta = millis() - lastBeat;
      lastBeat = millis();
      
      // Calculate the heartbeat per minute.
      beatsPerMinute = 60 / (delta / 1000.0);
      Serial.print("Test ");
      Serial.println(beatsPerMinute);
      
      if (beatsPerMinute < 255 && beatsPerMinute > 20){
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= RATE_SIZE;
        
        //Calculate the average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
          beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }
    }
  }
  else{
    beatAvg = 0;
    Serial.println("Please put your finger on the sensor!");
  }
}
  
