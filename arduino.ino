// Note that this code is not the final code and there will be changes made
// This code also might not work because the serial communications for some reason does not work and instead just flashes the LED a couple of times

void setup() {
  Serial.begin(9600); // begin serial communications
  pinMode(LED_BUILTIN, OUTPUT); // set the builtin LED as an output LED
  while (!Serial) {
    ;
  }
}

void loop() {
  char buffer[16];
  if (Serial.available() > 0) {
    int size = Serial.readBytesUntil('\n', buffer, 12);
    if (buffer[0] == 'Y') { // if anything from serial is "Y", run the code below which turns the builtin LED on
      digitalWrite(LED_BUILTIN, HIGH); 
    }
    if (buffer[0] == 'N') { // if anything from serial is "N", run the code below which turns the builtin LED off
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
