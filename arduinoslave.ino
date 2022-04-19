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

int gled = 4;
int bled = 5;
int rled = 6;
// initialize the three LED's with different pins

void setup() {
  Serial.begin(11520);
  // begin baud rate at 115200 because 9600 did not work with serial communication with the Jetson Nano
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(rled, OUTPUT);
  // sets the pin mode for every LED to OUTPUT
  while (!Serial) { // waits for the serial port to connect
    ;
  }
}
void loop() {
  char buffer[16];
  if (Serial.available() > 0) { // checks if serial is available and if the data is more than 0
    int size = Serial.readBytesUntil('\n', buffer, 12);
    
    if (buffer[0] == 'G') { // this is statment checks if the first letter of the data is "G" and if so, run the code below which turns the green LED on
      digitalWrite(gled, HIGH);
    }
    else { // if there is no output, keep the LED off (this might not be needed)
      digitalWrite(gled, LOW);
    }
    
    if (buffer[0] == 'B') { // this is statment checks if the first letter of the data is "B" and if so, run the code below which turns the blue LED on
      digitalWrite(bled, HIGH);
    }
    else { // if there is no output, keep the LED off (this might not be needed)
      digitalWrite(bled, LOW);
    }

    if (buffer[0] == 'R') { // this is statment checks if the first letter of the data is "R" and if so, run the code below which turns the red LED on
      digitalWrite(rled, HIGH);
    }
    else { // if there is no output, keep the LED off (this might not be needed)
      digitalWrite(rled, LOW);
    }
    
  }
}