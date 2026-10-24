const int buttonPin = 2;
const int RledPin = 3;    
const int GledPin = 5;   
const int BledPin = 6;    
const int lightPin = A0; 

const bool COMMON_ANODE = true;

const bool LIGHT_INVERTED = true;

const int minMood = 0;
const int maxMood = 20;
const int neutralMood = 10;
int mood = 0;

const int darkThreshold = 400;    
const int veryDarkThreshold = 150;  
int lightValue = 0;                 


const int BRIGHT = 0;
const int DARK = 1;
const int VERY_DARK = 2;
int lightState = BRIGHT;

int buttonState = HIGH;
bool ButtonPressed = false;
const unsigned long debounceDelay = 30;
int lastReading = HIGH;
unsigned long lastDebounceTime = 0;

unsigned long touchedTimer = 0;    
unsigned long reducedTimer = 0;   
unsigned long darkStartTime = 0;   
unsigned long darkReduceTimer = 0; 
unsigned long printTimer = 0;      

const unsigned long unTouchInterval = 5000;  
const unsigned long reducedInterval = 1000;  
const unsigned long darkDelay = 3000;        
const unsigned long darkFastInterval = 500;  
const unsigned long veryDarkInterval = 1000;  
const unsigned long printInterval = 500;     

void writeRGB(float r, float g, float b) {
  int ri = (int)r, gi = (int)g, bi = (int)b;
  if (!COMMON_ANODE) {
    ri = 255 - ri;
    gi = 255 - gi;
    bi = 255 - bi;
  }
  analogWrite(RledPin, ri);
  analogWrite(GledPin, gi);
  analogWrite(BledPin, bi);
}

void showLEDState(int m) {
  float brightnessInterval = 255 / 10.0;

  if (m >= neutralMood) {
    float step = brightnessInterval * (m - neutralMood);
    writeRGB(255, step, 255 - step);
  } else {
    float step = brightnessInterval * (neutralMood - m);
    writeRGB(255 - step, step, 255);
  }
}

int readButton() {
  int reading = digitalRead(buttonPin);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  lastReading = reading;

  if (millis() - lastDebounceTime > debounceDelay) {
    buttonState = reading;
  }
  return buttonState;
}

void updateLight() {
  int raw = analogRead(lightPin);
  if (LIGHT_INVERTED) raw = 1023 - raw;

  lightValue = (lightValue * 4 + raw) / 5;

  int newState;
  if (lightValue < veryDarkThreshold) newState = VERY_DARK;
  else if (lightValue < darkThreshold) newState = DARK;
  else newState = BRIGHT;

  if (lightState == BRIGHT && newState != BRIGHT) {
    darkStartTime = millis();
    darkReduceTimer = millis();
  }
  lightState = newState;
}

const char* lightStateName() {
  if (lightState == VERY_DARK) return "Very dark";
  if (lightState == DARK) return "Dark";
  return "Bright";
}

void updateMoodDecay() {
  unsigned long now = millis();

  if (lightState == VERY_DARK) {
    if (now - darkReduceTimer >= veryDarkInterval) {
      mood -= 2;
      darkReduceTimer = now;
    }
  } else if (lightState == DARK && now - darkStartTime > darkDelay) {
    if (now - darkReduceTimer >= darkFastInterval) {
      mood -= 1;
      darkReduceTimer = now;
    }
  } else {
    if (now - touchedTimer > unTouchInterval) {
      if (now - reducedTimer > reducedInterval) {
        mood -= 1;
        reducedTimer = now;
      }
    }
  }

  if (mood < minMood) mood = minMood;
}

void printStatus() {
  Serial.print("Light: ");
  Serial.print(lightValue);
  Serial.print("\t State: ");
  Serial.print(lightStateName());
  Serial.print("\t Mood: ");
  Serial.println(mood);
}

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);  
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  mood = neutralMood;
  touchedTimer = millis();
  reducedTimer = millis();
  lightValue = analogRead(lightPin);
  if (LIGHT_INVERTED) lightValue = 1023 - lightValue;

  Serial.println("=== Mood Toy Start ===");
}

void loop() {
  updateLight();
  showLEDState(mood);

  int state = readButton();

  if (state == LOW && !ButtonPressed) {
    mood++;
    if (mood > maxMood) mood = maxMood;
    touchedTimer = millis();
    ButtonPressed = true;
    Serial.print(">> Pet! Mood: ");
    Serial.println(mood);
  }
  if (state == HIGH && ButtonPressed) {
    ButtonPressed = false;
  }

  updateMoodDecay();

  if (millis() - printTimer >= printInterval) {
    printStatus();
    printTimer = millis();
  }
}
