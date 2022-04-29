//MIT License

//Copyright (c) 2022 324Hz

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS (WIN21H2, 324HZ) OR COPYRIGHT HOLDERS (WIN21H2, 324HZ) BE LIABLE FOR
//ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
//OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
//USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <SoftwareSerial.h>
#include <AccelStepper.h>
#include<Wire.h>
// include any libraries

#define dirPin 45
#define stepPin 44
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
// setup the interface and the dir/step pins for the DRV8825 motor driver

void setup() {
  stepper.setMaxSpeed(1000);
  // set the max speed the motor can go at
  Serial.begin(11520);
  // begin baud rate at 11520
  while (!Serial) { // waits for the serial port to connect
    ;
  }
}

void ledoff(int lednum) {
  delay(2000);
  stepper.setSpeed(0);
  stepper.runSpeed();
}
// attempting to keep the stepper motor on for 2000 miliseconds although this does not work and the stepper motor only makes super small steps but never stays on

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    // the above is the same as the "arduinoledtest" file
    
    if (buffer[0] == 'R') {
      stepper.setSpeed(400);
      stepper.runSpeed();
      // the same code as the "arduinoledtest" file although here we set a speed for the motor and run the motor at that speed
    }
    
    if (buffer[0] == 'B') {
      stepper.setSpeed(0);
      stepper.runSpeed();
      // this just keeps the stepper motor off
    }
  }
}
