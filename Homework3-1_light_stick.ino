const int buttonPin = 2;
const int RLedPin = 9;
const int GLedPin = 10;
const int BLedPin = 11;

const bool COMMON_ANODE = true;

const unsigned long debounceDelay = 30;
const unsigned long longPressInterval = 2000;
int lastReading = HIGH;        
int stableState = HIGH;        
unsigned long lastDebounceTime = 0;
unsigned long pressingTime = 0;

const int NUM_MODES = 5;
int currentMode = 0;

const int NUM_COLORS = 5;
int lightNum = 0;
// 0:白 1:紅 2:綠 3:藍 4:黃
const byte palette[NUM_COLORS][3] = {
  {255, 255, 255},
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255},
  {255, 255, 0}
};
int RLightColor = 255;
int GLightColor = 255;
int BLightColor = 255;

int RCurrentColor = 0;
int GCurrentColor = 0;
int BCurrentColor = 0;

// ---- 閃爍模式 ----
const unsigned long blinkInterval = 500;
unsigned long blinkTimer = 0;
bool blinkOn = true;

// ---- 呼吸模式 ----
const int breathStep = 3;                 // 每次亮度變化量
const unsigned long breathInterval = 15;  // 更新間隔 (ms)
int breathLevel = 0;                      // 0~255
int breathDirection = 1;
unsigned long breathTimer = 0;

// ---- 彩虹模式 ----
const unsigned long rainbowInterval = 15; // 數字越小轉得越快
int rainbowHue = 0;                       // 0~359
unsigned long rainbowTimer = 0;

// ---- 心跳模式 ----
const unsigned long heartPeriod = 1000;   // 一次心跳週期 (ms)
unsigned long heartStart = 0;

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(RLedPin, OUTPUT);
  pinMode(GLedPin, OUTPUT);
  pinMode(BLedPin, OUTPUT);
  applyPalette();
}

void loop()
{
  checkButton();
  updateLEDColor();
  setRGBLEDColor(RCurrentColor, GCurrentColor, BCurrentColor);
}

void checkButton()
{
  int reading = digitalRead(buttonPin);

  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if (millis() - lastDebounceTime > debounceDelay && reading != stableState) {
    stableState = reading;

    if (stableState == LOW) {
      pressingTime = millis();
    } else {
      if (millis() - pressingTime < longPressInterval) {
        changeLEDColor();  
      } else {
        changeMode();     
      }
    }
  }
}

void changeMode()
{
  currentMode = (currentMode + 1) % NUM_MODES;

  switch (currentMode) {
    case 1:  // 閃爍
      blinkTimer = millis();
      blinkOn = true;
      break;
    case 2:  // 呼吸
      breathLevel = 0;
      breathDirection = 1;
      breathTimer = millis();
      break;
    case 3:  // 彩虹
      rainbowHue = 0;
      rainbowTimer = millis();
      break;
    case 4:  // 心跳
      heartStart = millis();
      break;
  }
}

void changeLEDColor()
{
  lightNum = (lightNum + 1) % NUM_COLORS;
  applyPalette();
}

void applyPalette()
{
  RLightColor = palette[lightNum][0];
  GLightColor = palette[lightNum][1];
  BLightColor = palette[lightNum][2];
  RCurrentColor = RLightColor;
  GCurrentColor = GLightColor;
  BCurrentColor = BLightColor;
}

void updateLEDColor()
{
  unsigned long now = millis();

  switch (currentMode) {

    case 0:  // 恆亮
      RCurrentColor = RLightColor;
      GCurrentColor = GLightColor;
      BCurrentColor = BLightColor;
      break;

    case 1:  // 閃爍
      if (now - blinkTimer >= blinkInterval) {
        blinkOn = !blinkOn;
        blinkTimer = now;
      }
      if (blinkOn) {
        RCurrentColor = RLightColor;
        GCurrentColor = GLightColor;
        BCurrentColor = BLightColor;
      } else {
        RCurrentColor = 0;
        GCurrentColor = 0;
        BCurrentColor = 0;
      }
      break;

    case 2:  // 呼吸燈
      if (now - breathTimer >= breathInterval) {
        breathTimer = now;
        breathLevel += breathDirection * breathStep;
        if (breathLevel >= 255) {
          breathLevel = 255;
          breathDirection = -1;
        } else if (breathLevel <= 0) {
          breathLevel = 0;
          breathDirection = 1;
        }
      }
      scaleColor(breathLevel);
      break;

    case 3:  // 彩虹
      if (now - rainbowTimer >= rainbowInterval) {
        rainbowTimer = now;
        rainbowHue = (rainbowHue + 1) % 360;
      }
      hueToRGB(rainbowHue);
      break;

    case 4:  // 心跳
      scaleColor(heartbeatBrightness((now - heartStart) % heartPeriod));
      break;
  }
}

void scaleColor(int level)
{
  RCurrentColor = RLightColor * level / 255;
  GCurrentColor = GLightColor * level / 255;
  BCurrentColor = BLightColor * level / 255;
}
int heartbeatBrightness(unsigned long t)
{
  long x = (long)t;
  if (x < 60)  return map(x, 0, 60, 0, 255);     // 第一下：快速亮起
  if (x < 140) return map(x, 60, 140, 255, 40);  // 快速變暗
  if (x < 200) return map(x, 140, 200, 40, 200); // 第二下：再亮起
  if (x < 340) return map(x, 200, 340, 200, 0);  // 慢慢熄滅
  return 0;                                       // 休息
}

void hueToRGB(int h)
{
  int region = h / 60;
  int x = (h % 60) * 255 / 60;

  switch (region) {
    case 0: RCurrentColor = 255;     GCurrentColor = x;       BCurrentColor = 0;       break;
    case 1: RCurrentColor = 255 - x; GCurrentColor = 255;     BCurrentColor = 0;       break;
    case 2: RCurrentColor = 0;       GCurrentColor = 255;     BCurrentColor = x;       break;
    case 3: RCurrentColor = 0;       GCurrentColor = 255 - x; BCurrentColor = 255;     break;
    case 4: RCurrentColor = x;       GCurrentColor = 0;       BCurrentColor = 255;     break;
    default: RCurrentColor = 255;    GCurrentColor = 0;       BCurrentColor = 255 - x; break;
  }
}

void setRGBLEDColor(int r, int g, int b)
{
  r = constrain(r, 0, 255);
  g = constrain(g, 0, 255);
  b = constrain(b, 0, 255);

  if (COMMON_ANODE) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }

  analogWrite(RLedPin, r);
  analogWrite(GLedPin, g);
  analogWrite(BLedPin, b);
}
