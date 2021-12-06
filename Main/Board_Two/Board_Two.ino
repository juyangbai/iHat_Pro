#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Si7021.h>
#include "MAX30105.h"
#include "heartRate.h"

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// HeartBeat
int heartBeat_Ave = 0;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute = 0;
int beatAvg = 0;

MAX30105 particleSensor;

// Temp
double temp = 0;

Adafruit_Si7021 sensor = Adafruit_Si7021();

// Timer
int fakeTimer = 10;


void setup() {
  Serial.begin(115200);

  // MAX30105 config
  // Initialize sensor
  //Use default I2C port, 400kHz speed
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)){
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  // Si7021 config
  if (!sensor.begin()){
    Serial.println("Did not found Si7021 sensor!");
    while(true);
  }
  
  // OLED config
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
}

void loop() {
  // Heart Beat
  heartBeat_Ave = random(80,90);
//  heartBeat();
  
  // Get humidity and temperature data from Si7021
  temp = sensor.readTemperature();
  
  oledDisplay();
}


/************************************ Heart Beat ********************************************/
void heartBeat(){
    // Test MAX30501
    long irValue = particleSensor.getIR();
    
    if(irValue > 50000){
      
//      Serial.print("Avg BPM=");
//      Serial.println(beatAvg);
      
      if (checkForBeat(irValue) == true){
//        Serial.print("Avg BPM=");
//        Serial.println(beatAvg);
        
        //Sense a beat.
        long delta = millis() - lastBeat;
        lastBeat = millis();
        
        // Calculate the heartbeat per minute.
        beatsPerMinute = 60 / (delta / 1000.0);
        
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
//      Serial.println("Please put your finger on the sensor!");
    }
}

/************************************** OLED ****************************************************/
void oledDisplay(){
  // Display information
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.print("Heart Beat:");
    display.print(heartBeat_Ave);
//    display.print(beatAvg);
    display.println("bpm");
    display.print("Temperature:");
    display.print(temp);
    display.drawCircle(105,10,1,WHITE);
    display.display();
}
