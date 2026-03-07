#include <Adafruit_GFX.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include <Bonezegei_DHT11.h>

#define TEMPSENSOR 2
#define FAN 6
#define CLK A5
#define LCD A4

#define timeOn 15 //15 minutes on
#define timeOff 30 // for every 30 minutes
Adafruit_7segment matrix = Adafruit_7segment();
Bonezegei_DHT11 dht(TEMPSENSOR);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(FAN, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LCD, OUTPUT);
  dht.begin();
  matrix.begin(0x70);

}
uint16_t counter = 0; //counter that increments every second

void loop() {
  // put your main code here, to run repeatedly:

   if (dht.getData() && counter % 5 == 0) {                         // get All data from DHT11 every 5 seconds
    float tempDeg = dht.getTemperature();      // return temperature in celsius
    float tempFar = dht.getTemperature(true);  // return temperature in fahrenheit if true celsius of false
    int hum = dht.getHumidity();               // return humidity
    String str  = "Temperature: ";
           str += tempDeg;
           str += "°C  ";
           str += tempFar;
           str += "°F  Humidity: ";
           str += hum;
           str += "Counter: ";
           str += counter;
    Serial.println(str.c_str());
    // Print the temp in the first two sections, and the humidity in the second two
    int displayValue = (int)tempFar*100 + hum;
    matrix.print(displayValue, DEC);
    matrix.drawColon(1);
    matrix.writeDisplay();
    //Serial.printf("Temperature: %0.1lf°C  %0.1lf°F Humidity:%d \n", tempDeg, tempFar, hum);
    
  }


  counter += 1;
  // We want the fan to turn on for 30 minutes out of the hour
  if (counter <= timeOn*60){
    analogWrite(FAN, 150);
  }
  else if (counter >= timeOn*60 && counter <= timeOff*60){
    analogWrite(FAN, 0);
  }
  else if (counter > timeOff*60){
    counter = 0;
  }
  delay(1000);

}
