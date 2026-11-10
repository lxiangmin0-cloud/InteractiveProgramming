const int buttonPin = 2;   
const int ledPin = 3;     

int mode = 0;             
int lastButtonState = HIGH;
unsigned long lastBlink = 0;
bool ledState = HIGH;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  
  int reading = digitalRead(buttonPin);
  if (lastButtonState == HIGH && reading == LOW) {
    mode = (mode + 1) % 4;   
    delay(50);             
  }
  lastButtonState = reading;

  
  unsigned long interval = 0;
  if (mode == 0) {
    digitalWrite(ledPin, HIGH);
    return;
  } else if (mode == 1) interval = 1000;  // 慢閃
  else if (mode == 2) interval = 500;     // 中閃
  else if (mode == 3) interval = 150;     // 快閃

  if (millis() - lastBlink >= interval) {
    lastBlink = millis();
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}