#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute = 0;
int beatAvg = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("Initializing...");
  
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
}

void loop(){
  
  long irValue = particleSensor.getIR();

  if(irValue > 50000){

    Serial.print("Avg BPM=");
    Serial.println(beatAvg);
    
    if (checkForBeat(irValue) == true){
      Serial.print("Avg BPM=");
      Serial.println(beatAvg);
      
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
    
//    Serial.print("IR=");
//    Serial.print(irValue);
//    Serial.print(", BPM=");
//    Serial.print(beatsPerMinute);
//    Serial.print(", Avg BPM=");
//    Serial.print(beatAvg);
    
  }
  else{
    beatAvg = 0;
    Serial.println("Please put your finger on the sensor!");
  }
//  Serial.println();
}
