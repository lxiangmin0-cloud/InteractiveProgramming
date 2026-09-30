const int pushButton = 2;
const int ledPin = 3;

int bottonState = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(pushButton, INPUT);
}

// the loop function runs over and over again forever
void loop() {
 bottonState = digitalRead(pushButton);

 if (bottonState == HIGH) {

  digitalWrite(ledPin, LOW);
 } else {
  digitalWrite(ledPin, HIGH);
 }
}
