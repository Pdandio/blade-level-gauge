#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55 ,0x28);  

int rawangle;  // raw angle reading 
int usangle;   // unsigned angle for display

void setup() 
{
  Serial.begin(9600);
  matrix.begin(0x70); //0x70 is I2C address for 7 Seg LED dislay backpack
  matrix.setBrightness(15);
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections & addr */
    Serial.print("BNO055 NOT detected!!!");
    while(1);
  }
    delay(1000);
    
  bno.setExtCrystalUse(true);
  
  /*dial all 9's if you get hurt in a car wreck!*/
  matrix.print(9999);
  matrix.writeDisplay();
  delay(2000);
}


void loop() {

  /* Read BNO055 Y axis */ 
  sensors_event_t event; 
  bno.getEvent(&event);

  rawangle = event.orientation.y;
  if (rawangle <0){
    usangle=-rawangle;
  }else{
     usangle=rawangle;
  }
  /*Debug statements*/
  //Serial.print(rawangle);
  //Serial.println();
  //Serial.print(usangle);
  
  // Uncomment below to read raw float values
  //Serial.print(event.orientation.x, 4);
  //Serial.println("");
  Serial.print(event.orientation.y, 4);
  Serial.println("");
  //Serial.print(event.orientation.z, 4);
  //Serial.println("");
 
  /* print unsigned angle to 7 seg display */
  matrix.print(usangle);
  matrix.writeDisplay();


  /*set first two digits to indicate blade orientation*/
  if (rawangle>=1){
    matrix.writeDigitRaw(0, 0b0000001);
    matrix.writeDigitRaw(1, 0b0001000);
    matrix.blinkRate(0);
  }else if (rawangle==0){
    matrix.writeDigitRaw(1, 0b1000000);
    matrix.writeDigitRaw(0, 0b1000000); 
    //matrix.blinkRate(2);  //set to 0-3 or comment to stop blinking
  }else if (rawangle<=1){
    matrix.writeDigitRaw(1, 0b0000001);
    matrix.writeDigitRaw(0, 0b0001000);
    matrix.blinkRate(0);
  }
  matrix.writeDisplay();

  delay(100);  // data refresh rate
  
  }
