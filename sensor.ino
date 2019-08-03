#include "Wire.h"                   // Communication with Gy521-sensormodules via I2C-Bus 
#include <stdlib.h>                 // here needed to use "atoi()" in particular;

const int MPUlow = 0x68;            // I2C-Bus-adresses of the two used Gy-521 modules
const int MPUup  = 0x69;
int16_t xL,yL,zL,xU,yU,zU;          // variables für acceleration values

int posL;                           // variables to store the calculated servo positions
int posU;
int handPin = A0;                   // Pin to read analog values from potentiometer 
int handAlt;                        // compare old and new values
int handNeu;                        // to detect movement of fingers 

String positions;                   // String to store and send data 

char tmp_str[7];                    // variable to convert the int16_t-values read from the registers on Gy521

char* convert(int16_t i){           // convert int16_t-values into an string
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

void setup() {

  Serial.begin(9600);                 // serial communication with BL-modul
  Wire.begin();                       // communication with gy-modules via I2C-Bus
  Wire.beginTransmission(MPUlow);
  Wire.write(0x6B);
  Wire.write(0);
  Serial.println("MPUlow connected!");
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPUup);
  Wire.write(0x6B);
  Wire.write(0);
  Serial.println("MPUup connected!");
  Wire.endTransmission(true);

  handNeu = analogRead(handPin);
  handAlt = handNeu;

}

void loop() {
                                        // get sensorvalues from sensor attached to forearm (corresponds to lower part of the robot)
  Wire.beginTransmission(MPUlow);       // start communication   
  Wire.write(0x3B);                     // first accelerometer-register
  Wire.endTransmission(false);
  Wire.requestFrom(MPUlow, 3*2, true);  // read values from the 3*2 accelerometer-registers 
  xL = Wire.read() << 8 | Wire.read();
  yL = Wire.read() << 8 | Wire.read();
  zL = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);

  Wire.beginTransmission(MPUup);        // get values from sensor attached to back of the hand 
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPUup, 3*2, true);
  xU = Wire.read() << 8 | Wire.read();
  yU = Wire.read() << 8 | Wire.read();
  zU = Wire.read() << 8 | Wire.read();
  Wire.endTransmission(true);

  posL = atoi(convert(xL));           // convert to int
  posU = atoi(convert(xU));

  posL = (posL/1000) * 7;             // calculate motorposition from acceleration values caused by gravity, we derived the rule by collecting experimental data
  posU = (posU/1000) * 7;      


  if(posL > 108){                     // 90° corresponds to position 108, maximum of motion range
    posL = 113;
  }else if(posL < 0){                 // we dont allow negative angles
    posL = 0;
  }
  if(posU > 108){                     // 90° corresponds to position 108, maximum of motion range
    posU = 113;
  }else if(posU < 0){
    posU = 0;
  }

/* Bulid information block of form: H LLL UUU; hand, lower part (forearm), upper part (back of the hand) */
  
  positions = "";

  handNeu = analogRead(handPin);

  if(handNeu > handAlt + 5){            // Threshold form finger movement
    positions = "0";
  }else if(handNeu < handAlt - 5){
    positions = "2";
  }else {
    positions = "1";
  }

  handAlt = handNeu; 
  
  if (posL < 100 && posL > 9){          // add zeros if necessary to maintain form of seven digits
    positions = positions + "0";
    positions = positions + posL;
  }else if ( posL < 10){
    positions = positions + "00";
    positions = positions + posL;
  } else {
    positions = positions + posL;
  }

  if (posU < 100 && posU > 9){
    positions = positions + "0";
    positions = positions + posU;
    Serial.println(positions);          // send information block via BL to motor unit
  }else if ( posU < 10){
   positions = positions + "00";
    positions = positions + posU;
    Serial.println(positions);
  } else {
    positions = positions + posU;
    Serial.println(positions);
  }
 
  delay(100);                         // measurement frequency around 10 Hz

}
