int rled = 2;
int gled = 3;

void setup() {
  Serial.begin(11520);
  pinMode(rled, OUTPUT);
  pinMode(gled, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      case 'R':
        digitalWrite(rled, HIGH);
        digitalWrite(gled, LOW);
        break;
      case 'B':
        digitalWrite(rled, LOW);
        digitalWrite(gled, HIGH);
        break;
      case 'O':
        digitalWrite(rled, LOW);
        digitalWrite(gled, LOW);
        break;
    }
  }
}











/* 0 = 0
 * - = 
 * 1 = wheelSpeed
 * 
 * 
 * "R" or "r" = STOP
 * 0
 * 0
 * 0
 * 0
 * 
 * "B" or "b" = GO (FORWARDS)
 * -
 * 1
 * 1
 * -
*/

#include <SoftwareSerial.h>
#include <AccelStepper.h>

SoftwareSerial Bluetooth(A5, 0);
AccelStepper LeftBackWheel(1, 3, 2); 
AccelStepper LeftFrontWheel(1, 5, 4); 
AccelStepper RightBackWheel(1, 7, 6);  
AccelStepper RightFrontWheel(1, 9, 8); 

int wheelSpeed = 1500;
int dataIn, m;
int lbw[50], lfw[50], rbw[50], rfw[50]; 
int index = 0;

void setup() {
  LeftFrontWheel.setMaxSpeed(3000);
  LeftBackWheel.setMaxSpeed(3000);
  RightFrontWheel.setMaxSpeed(3000);
  RightBackWheel.setMaxSpeed(3000);
  
  Serial.begin(38400);
  Bluetooth.begin(9600); 
}

void loop() {
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.read();  
    if (dataIn == 0) {
      m = 0;
      Serial.println("dataIn0");
    }
    if (dataIn == 2) {
      m = 2;
      Serial.println("dataIn2");
    }
    if (dataIn == 4) {
      m = 4;
      Serial.println("dataIn4");
    }
    if (dataIn == 5) {
      m = 5;
      Serial.println("dataIn5");
    }
    if (dataIn == 7) {
      m = 7;
      Serial.println("dataIn7");
    }
    if (dataIn == 9) {
      m = 9;
      Serial.println("dataIn9");
    }
    if (dataIn == 10) {
      m = 10;
      Serial.println("dataIn10");
    }
    
    if (dataIn >= 16) {
      wheelSpeed = dataIn * 10;
      Serial.println(wheelSpeed);
    }
  }

  if (m == 0) {
    stopMoving();
  }
  if (m == 2) {
    moveForward();
  }
  if (m == 4) {
    moveSidewaysLeft();
  }
  if (m == 5) {
    moveSidewaysRight();
  }
  if (m == 7) {
    moveBackward();
  }
  if (m == 9) {
    rotateLeft();
  }
  if (m == 10) {
    rotateRight();
  }
  
  if (m == 12) {
    if (index == 0) {
      LeftBackWheel.setCurrentPosition(0);
      LeftFrontWheel.setCurrentPosition(0);
      RightBackWheel.setCurrentPosition(0);
      RightFrontWheel.setCurrentPosition(0);
    }
    lbw[index] = LeftBackWheel.currentPosition(); 
    lfw[index] = LeftFrontWheel.currentPosition();
    rbw[index] = RightBackWheel.currentPosition();
    rfw[index] = RightFrontWheel.currentPosition();
    index++;
    m = 0;
  }

  if (m == 14) {
    runSteps();
    if (dataIn != 14) {
      stopMoving();
      memset(lbw, 0, sizeof(lbw));
      memset(lfw, 0, sizeof(lfw));
      memset(rbw, 0, sizeof(rbw));
      memset(rfw, 0, sizeof(rfw));
      index = 0;
    }
  }

  LeftFrontWheel.runSpeed();
  LeftBackWheel.runSpeed();
  RightFrontWheel.runSpeed();
  RightBackWheel.runSpeed();
}

void runSteps() {
  for (int i = index - 1; i >= 0; i--) { 
    LeftFrontWheel.moveTo(lfw[i]);
    LeftFrontWheel.setSpeed(wheelSpeed);
    LeftBackWheel.moveTo(lbw[i]);
    LeftBackWheel.setSpeed(wheelSpeed);
    RightFrontWheel.moveTo(rfw[i]);
    RightFrontWheel.setSpeed(wheelSpeed);
    RightBackWheel.moveTo(rbw[i]);
    RightBackWheel.setSpeed(wheelSpeed);

    while (LeftBackWheel.currentPosition() != lbw[i] & LeftFrontWheel.currentPosition() != lfw[i] & RightFrontWheel.currentPosition() != rfw[i] & RightBackWheel.currentPosition() != rbw[i]) {
      LeftFrontWheel.runSpeedToPosition();
      LeftBackWheel.runSpeedToPosition();
      RightFrontWheel.runSpeedToPosition();
      RightBackWheel.runSpeedToPosition();

      if (Bluetooth.available() > 0) {    
        dataIn = Bluetooth.read();
        if ( dataIn == 15) {       
          while (dataIn != 14) { 
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.read();
              if ( dataIn == 13) {
                stopMoving();
                break;
              }
            }
          }
        }
        if (dataIn >= 16) {
          wheelSpeed = dataIn * 10;
          dataIn = 14;
        }
        if ( dataIn == 13) {
          break;
        }
      }
    }
  }

  for (int i = 1; i <= index - 1; i++) {

    LeftFrontWheel.moveTo(lfw[i]);
    LeftFrontWheel.setSpeed(wheelSpeed);
    LeftBackWheel.moveTo(lbw[i]);
    LeftBackWheel.setSpeed(wheelSpeed);
    RightFrontWheel.moveTo(rfw[i]);
    RightFrontWheel.setSpeed(wheelSpeed);
    RightBackWheel.moveTo(rbw[i]);
    RightBackWheel.setSpeed(wheelSpeed);

    while (LeftBackWheel.currentPosition() != lbw[i]& LeftFrontWheel.currentPosition() != lfw[i] & RightFrontWheel.currentPosition() != rfw[i] & RightBackWheel.currentPosition() != rbw[i]) {
      LeftFrontWheel.runSpeedToPosition();
      LeftBackWheel.runSpeedToPosition();
      RightFrontWheel.runSpeedToPosition();
      RightBackWheel.runSpeedToPosition();

      if (Bluetooth.available() > 0) {    
        dataIn = Bluetooth.read();
        if ( dataIn == 15) {          
          while (dataIn != 14) {       
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.read();
              if ( dataIn == 13) {
                stopMoving();
                break;
              }
            }
          }
        }
        if (dataIn >= 16) {
          wheelSpeed = dataIn * 10;
          dataIn = 14;
        }
        if ( dataIn == 13) {
          break;
        }
      }
    }
  }
}

void moveForward() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveBackward() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void moveSidewaysRight() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveSidewaysLeft() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void rotateLeft() {
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void rotateRight() {
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void stopMoving() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(0);
}
