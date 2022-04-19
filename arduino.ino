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
