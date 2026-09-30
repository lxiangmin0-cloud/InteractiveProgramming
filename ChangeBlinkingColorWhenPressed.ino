const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;

int buttonState = 0;
int ledcolor = 0; 
int ledState = LOW;
bool ButtonPressed = false;
String currentcolor = "led";

unsigned long previousMillis = 0; 
const long interval = 1000;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
 
  buttonState = digitalRead(buttonPin);
  Serial.print("Current Color: ");
  Serial.println(currentcolor);

  if (buttonState == HIGH && !ButtonPressed) {
    ledcolor = ledcolor + 1;
    ButtonPressed = true;
  }
  if (buttonState == LOW && ButtonPressed) {
    ButtonPressed = false;
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
  
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }

  if (ledcolor == 0) {
    currentcolor = "LED off";
    digitalWrite(RledPin, HIGH);
    digitalWrite(GledPin, HIGH);
    digitalWrite(BledPin, HIGH);
  }
  else if (ledcolor == 1) {
    // RED
    currentcolor = "Red";
    if (ledState == LOW) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  else if (ledcolor == 2) {
    // Green
    currentcolor = "Green";
    if (ledState == LOW) {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  // BLUE
  else if (ledcolor == 3) {
    currentcolor = "Blue";
    if (ledState == LOW) {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, LOW);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  // yellow
  else if (ledcolor == 4) {
    currentcolor = "Yellow";
    if (ledState == LOW) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  // purple
  else if (ledcolor == 5) {
    currentcolor = "Purple";
    if (ledState == LOW) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, LOW);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  // cyan
  else if (ledcolor == 6) {
    currentcolor = "Cyan";
    if (ledState == LOW) {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, LOW);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  // white
  else if (ledcolor == 7) {
    currentcolor = "White";
    if (ledState == LOW) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, LOW);
    } else {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    }
  }
  else if (ledcolor == 8) {
    ledcolor = 0;
  }
}
