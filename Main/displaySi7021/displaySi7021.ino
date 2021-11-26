#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Si7021.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     4
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_Si7021 sensor = Adafruit_Si7021();

void setup() {
  // OLED config
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Si7021 config
  if (!sensor.begin()){
    Serial.println("Did not found Si7021 sensor!");
    while(true);
  }
}

void loop() {
  // Get humidity and temperature data from Si7021
  double humid = sensor.readHumidity();
  double temp = sensor.readTemperature();
  
  // Display information
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.print("Humidity:");
  display.print(humid);
  display.println("%");
  display.print("Temperature:");
  display.print(temp);
  display.drawCircle(105,10,1,WHITE);
  display.display();
}
