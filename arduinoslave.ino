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

void setup() {
  Serial.begin(11520);
  pinMode(gled, OUTPUT);
  pinMode(bled, OUTPUT);
  pinMode(rled, OUTPUT);
  while (!Serial) {
    ;
  }
}
void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    
    if (buffer[0] == 'G') {
      digitalWrite(gled, HIGH);
    }
    else {
      digitalWrite(gled, LOW);
    }
    
    if (buffer[0] == 'B') {
      digitalWrite(bled, HIGH);
    }
    else {
      digitalWrite(bled, LOW);
    }

    if (buffer[0] == 'R') {
      digitalWrite(rled, HIGH);
    }
    else {
      digitalWrite(rled, LOW);
    }
  }
}
