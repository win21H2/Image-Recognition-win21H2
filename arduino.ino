void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) {
    ;
  }
}

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    if (buffer[0] == 'Y') {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (buffer[0] == 'N') {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
