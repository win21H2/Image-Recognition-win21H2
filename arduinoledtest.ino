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

int bled = 5;
int rled = 6;
// initialize the three LED's with different pins

void setup() {
  Serial.begin(11520);
  // begin baud rate at 115200 because 9600 did not work with serial communication with the Jetson Nano
  pinMode(bled, OUTPUT);
  pinMode(rled, OUTPUT);
  // sets the pin mode for every LED to OUTPUT
  while (!Serial) { // waits for the serial port to connect
    ;
  }
}

// We are trying now to see if we can get a void function to run which keeps the LED on after a set time. For now any changes made here will not affect the
// blinking we are already getting from the Arduino.
 
void ledoff(int lednum) {
  delay(2000);
  digitalWrite(lednum, LOW);
}

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    
    if (buffer[0] == 'G') {
      gledON = !gledON;
      digitalWrite(gled, gledON);
    }
    
    if (buffer[0] == 'R') {
      rledON = !rledON;
      digitalWrite(rled, rledON);  
    }
    /*
    Logic tree below
    If "R" comes from the output => turn RLED on => keep RLED on
    If "G" comes from the output => turn RLED off => turn GLED on => keep GLED on
    If "G" comes from the output (THIS IS WHEN THE JETSON RESENDS) => keep GLED on
    */

    if (received_code != previous_code ) {
        // do stuff here
        previous_code=received_code;
    }
  }
}
