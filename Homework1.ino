
const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;

int buttonState = HIGH;
int ledcolor = 0;

bool ButtonPressed = false;
bool autoMode = false;
bool longPressHandled = false;

String currentcolor = "LED off";

unsigned long pressStartTime = 0;
unsigned long lastColorChange = 0;

const unsigned long longPressDuration = 2000;
const unsigned long colorInterval = 1000;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  showColor();
}

void loop() {
  
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !ButtonPressed) {
    ButtonPressed = true;
    longPressHandled = false;
    pressStartTime = millis();
  }

  if (ButtonPressed &&
      !longPressHandled &&
      buttonState == LOW &&
      millis() - pressStartTime >= longPressDuration) {

    autoMode = !autoMode;
    longPressHandled = true;
    lastColorChange = millis();

    if (autoMode) {
      Serial.println("Auto Mode ON");
    } else {
      Serial.println("Auto Mode OFF");
    }
  }

  if (buttonState == HIGH && ButtonPressed) {
    ButtonPressed = false;

    if (!longPressHandled && !autoMode) {
      nextColor();
    }
  }

  if (autoMode &&
      millis() - lastColorChange >= colorInterval) {

    lastColorChange = millis();
    nextColor();
  }
}

void nextColor() {
  ledcolor++;

  if (ledcolor > 7) {
    ledcolor = 0;
  }

  showColor();
}

void showColor() {
  switch (ledcolor) {
    case 0:
      currentcolor = "LED off";
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
      break;

    case 1:
      currentcolor = "Red";
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
      break;

    case 2:
      currentcolor = "Green";
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
      break;

    case 3:
      currentcolor = "Blue";
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, LOW);
      break;

    case 4:
      currentcolor = "Yellow";
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
      break;

    case 5:
      currentcolor = "Purple";
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, LOW);
      break;

    case 6:
      currentcolor = "Cyan";
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, LOW);
      break;

    case 7:
      currentcolor = "White";
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, LOW);
      break;
  }

  Serial.print("Current Color: ");
  Serial.print(currentcolor);
  Serial.print(" | Mode: ");
  Serial.println(autoMode ? "Auto" : "Manual");
}
