#include <SoftwareSerial.h>
#include <AccelStepper.h>

SoftwareSerial Bluetooth(A5, 0);
AccelStepper w1(1, 3, 2); 
AccelStepper w2(1, 5, 4); 
AccelStepper w3(1, 7, 6);  
AccelStepper w4(1, 9, 8); 

int wheelSpeed = 1500;
int dataIn, m;
int lbw[50], lfw[50], rbw[50], rfw[50]; 
int index = 0;

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
      case 'R':
        Serial.println("RED");
        w1.setSpeed(-wheelSpeed);
        w2.setSpeed(wheelSpeed);
        w3.setSpeed(wheelSpeed);
        w4.setSpeed(-wheelSpeed);
        break;
      case 'B':
        Serial.println("BLUE");
        w1.setSpeed(wheelSpeed);
        w2.setSpeed(-wheelSpeed);
        w3.setSpeed(-wheelSpeed);
        w4.setSpeed(wheelSpeed);
        break;
      case 'O':
        Serial.println("OFF");
        w1.setSpeed(0);
        w2.setSpeed(0);
        w3.setSpeed(0);
        w4.setSpeed(0);
        break;
    }
  }
  
  if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.read();  
    if (dataIn == 0) {
      m = 0;
    }
    if (dataIn == 2) {
      m = 2;
    }
    if (dataIn == 4) {
      m = 4;
    }
    if (dataIn == 5) {
      m = 5;
    }
    if (dataIn == 7) {
      m = 7;
    }
    if (dataIn == 9) {
      m = 9;
    }
    if (dataIn == 10) {
      m = 10;
    }
    
    if (dataIn >= 16) {
      wheelSpeed = dataIn * 10;
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
      w1.setCurrentPosition(0);
      w2.setCurrentPosition(0);
      w3.setCurrentPosition(0);
      w4.setCurrentPosition(0);
    }
    lbw[index] = w1.currentPosition(); 
    lfw[index] = w2.currentPosition();
    rbw[index] = w3.currentPosition();
    rfw[index] = w4.currentPosition();
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
  w1.runSpeed();
  w2.runSpeed();
  w3.runSpeed();
  w4.runSpeed();
}

void runSteps() {
  for (int i = index - 1; i >= 0; i--) { 
    w1.moveTo(lfw[i]);
    w1.setSpeed(wheelSpeed);
    w2.moveTo(lbw[i]);
    w2.setSpeed(wheelSpeed);
    w3.moveTo(rfw[i]);
    w3.setSpeed(wheelSpeed);
    w4.moveTo(rbw[i]);
    w4.setSpeed(wheelSpeed);

    while (w1.currentPosition() != lbw[i] & w2.currentPosition() != lfw[i] & w3.currentPosition() != rfw[i] & w4.currentPosition() != rbw[i]) {
      w1.runSpeedToPosition();
      w2.runSpeedToPosition();
      w3.runSpeedToPosition();
      w4.runSpeedToPosition();

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
    w1.moveTo(lfw[i]);
    w1.setSpeed(wheelSpeed);
    w2.moveTo(lbw[i]);
    w2.setSpeed(wheelSpeed);
    w3.moveTo(rfw[i]);
    w3.setSpeed(wheelSpeed);
    w4.moveTo(rbw[i]);
    w4.setSpeed(wheelSpeed);

    while (w1.currentPosition() != lbw[i]& w2.currentPosition() != lfw[i] & w3.currentPosition() != rfw[i] & w4.currentPosition() != rbw[i]) {
      w1.runSpeedToPosition();
      w2.runSpeedToPosition();
      w3.runSpeedToPosition();
      w4.runSpeedToPosition();

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
  w1.setSpeed(wheelSpeed);
  w2.setSpeed(-wheelSpeed);
  w3.setSpeed(-wheelSpeed);
  w4.setSpeed(wheelSpeed);
}
void moveBackward() {
  w1.setSpeed(-wheelSpeed);
  w2.setSpeed(wheelSpeed);
  w3.setSpeed(wheelSpeed);
  w4.setSpeed(-wheelSpeed);
}
void moveSidewaysRight() {
  w1.setSpeed(-wheelSpeed);
  w2.setSpeed(-wheelSpeed);
  w3.setSpeed(wheelSpeed);
  w4.setSpeed(wheelSpeed);
}
void moveSidewaysLeft() {
  w1.setSpeed(wheelSpeed);
  w2.setSpeed(wheelSpeed);
  w3.setSpeed(-wheelSpeed);
  w4.setSpeed(-wheelSpeed);
}
void rotateLeft() {
  w1.setSpeed(wheelSpeed);
  w2.setSpeed(wheelSpeed);
  w3.setSpeed(wheelSpeed);
  w4.setSpeed(wheelSpeed);
}
void rotateRight() {
  w1.setSpeed(-wheelSpeed);
  w2.setSpeed(-wheelSpeed);
  w3.setSpeed(-wheelSpeed);
  w4.setSpeed(-wheelSpeed);
}
void stopMoving() {
  w1.setSpeed(0);
  w2.setSpeed(0);
  w3.setSpeed(0);
  w4.setSpeed(0);
}
