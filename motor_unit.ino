/* Code to control the motorunit. Data received from the sensorunit are send to the servos. */
int value;    // intermediate variable to store Ascii-coded digits of computed servo positions
String param; // Format of data in the string are H LLL UUU: hand-, lower part- and upper part of the arm position
int c;        // counter variable for piece of information: H LLL UUU

int motorH = 10;    // pin servo hand
int motorL = 11;    // pin servo lower part of the arm (forearm of the user)
int motorU = 12;    // pin servo upper part of the arm (back of the hand of the user)
int led = 13;       // led to signal initial connection to sensor unit
#include <Servo.h>
Servo servoH;       // used motors
Servo servoL;
Servo servoU;


int h;              // variables for motorpositions
int l;
int u;


void setup() {
  Serial.begin(9600);         // serial connection (to BL-modul)

  servoH.attach(motorH);      // initialize servos
  servoL.attach(motorL);
  servoU.attach(motorU);

  servoH.write(40);         // starting position of the hand: half way open

  pinMode(led, OUTPUT);           // check, if BL-modules connected. Deactivate LED if first chunk of data is received.
  while(!Serial.available() > 0){
      digitalWrite(led,HIGH);
  }
  digitalWrite(led,LOW);
}

void loop() {
  
if(Serial.available() > 0){ // check if new data at serial port
  

  value = Serial.read();    // read serial data (elementwise)

  if( value == 13 || value == 10){    // new line: 1310 is send before each unit of informations (ASCII-code for CR LF)
    param = "";                       // start new informationunit, 
    c = 0;                            // does't carry any information itself, do not increase counter variable here
  }
  else {

    value = value - 48;               // Ascii "48" corresponds to "0" in decimal
    param = param + String(value);    // built up full informationunit in form H LLL UUU from the single chunks

    c++;                              // next information chunk
    if(c == 7){

      switch(param.substring(0,1).toInt()){

        case 0: h = servoH.read() - 5          ; break;      // close hand relative to current position       
        case 1:                                  break;      // hold position
        case 2: h = servoH.read() + 5          ; break;      // open hand relative to current position  
        
      }      

      l = param.substring(1,4).toInt();                     // transfer forearm position of the user directly to the lower part of the robot arm
      u = l - param.substring(4,7).toInt();                 // transfer position of back of the hand (upper part of robotarm) with respect to the forerarm (lower part of the robot arm)
      
      servoH.write(h);          // steer the motors
      servoL.write(l);   
      servoU.write(u);
      
    }
    
  }

}
}
