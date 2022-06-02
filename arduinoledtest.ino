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
