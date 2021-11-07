
// Config
int PIR_pin = A0;
int PIR_state = HIGH;
int val = 0;

int calibrationTime = 60;

int minimumSecsLowForInactive = 5000;
long unsigned int timelow;

void setup() {
  pinMode(PIR_pin, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial

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


void loop(){
  PIR_state = digitalRead(PIR_pin);   // read sensor value
  if (PIR_state == HIGH) {           // check if the sensor is HIGH
    delay(500);                // delay 100 milliseconds 
    Serial.println("Motion detected!"); 
  } 
  else {
    delay(500);             // delay 200 milliseconds 
    Serial.println("Motion stopped!");
  }
}
