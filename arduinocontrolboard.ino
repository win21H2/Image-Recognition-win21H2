// Control board code
#include <SoftwareSerial.h>
#include <AccelStepper.h>

SoftwareSerial Bluetooth(A1, 4);
AccelStepper LeftBackWheel(1, 8, 9);
AccelStepper LeftFrontWheel(1, 4, 5);
AccelStepper RightBackWheel(1, 6, 7);
AccelStepper RightFrontWheel(1, 9, 11);

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
    }
    if (dataIn == 2) {
      m = 2;
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
    if (dataIn == 5) {
      m = 5;
    }
    if (dataIn == 4) {
      m = 4;
    }
    
    if (dataIn >= 16) {
      wheelSpeed = dataIn * 10;
      Serial.println(wheelSpeed);
    }
  }

  if (m == 2) {
    moveForward();
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
  if (m == 0) {
    stopMoving();
  }
  if (m == 5) {
    moveSidewaysRight();
  }
  if (m == 4) {
    moveSidewaysLeft();
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





// Serial communications code
int gled = 4;
int bled = 5;
int rled = 6;

void setup() {
  Serial.begin(11520);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(rled, OUTPUT);
  while (!Serial) {
    ;
  }
}

void ledoff(int lednum) {
  delay(2000);
  digitalWrite(lednum, LOW);
}

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    
    if (buffer[0] == 'G') {
      digitalWrite(gled, HIGH);
      ledoff(gled); 
       
    }
    
    if (buffer[0] == 'B') {
      digitalWrite(bled, HIGH);
      ledoff(bled);  
    }

    if (buffer[0] == 'R') {
      digitalWrite(rled, HIGH);
      ledoff(rled);  
    }
  }
}
