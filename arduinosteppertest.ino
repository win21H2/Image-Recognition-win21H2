#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include<Wire.h>
#define dirPin 45
#define stepPin 44
#define motorInterfaceType 1

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  stepper.setMaxSpeed(1000);
  Serial.begin(11520);
  while (!Serial) {
    ;
  }
}

void ledoff(int lednum) {
  delay(2000);
  stepper.setSpeed(0);
  stepper.runSpeed();
}

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    
    if (buffer[0] == 'R') {
      stepper.setSpeed(400);
      stepper.runSpeed();

    }
    
    if (buffer[0] == 'B') {
      stepper.setSpeed(0);
      stepper.runSpeed();
    }
  }
}
