#include <SoftwareSerial.h>
#include <AccelStepper.h>

SoftwareSerial Bluetooth(A5, 0);
AccelStepper w1(1, 3, 2); 
AccelStepper w2(1, 5, 4); 
AccelStepper w3(1, 7, 6);  
AccelStepper w4(1, 9, 8); 

int wheelSpeed = 1500;

void setup() {
  w1.setMaxSpeed(3000);
  w2.setMaxSpeed(3000);
  w3.setMaxSpeed(3000);
  w4.setMaxSpeed(3000);

  Serial.begin(11520);
  Bluetooth.begin(9600); 
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      case 'G': // GOFORWARD
        w1.setSpeed(-wheelSpeed);
        w2.setSpeed(wheelSpeed);
        w3.setSpeed(wheelSpeed);
        w4.setSpeed(-wheelSpeed);
        break;
      case 'B': // GOBACKWARD
        w1.setSpeed(wheelSpeed);
        w2.setSpeed(-wheelSpeed);
        w3.setSpeed(-wheelSpeed);
        w4.setSpeed(wheelSpeed);
        break;
      case 'R': // STOP
        w1.setSpeed(0);
        w2.setSpeed(0);
        w3.setSpeed(0);
        w4.setSpeed(0);
        break;
    }
  }
  w1.runSpeed();
  w2.runSpeed();
  w3.runSpeed();
  w4.runSpeed();
}
