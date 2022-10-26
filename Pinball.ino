#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP23XXX.h>

int ledNum[19][7] = {
  {LOW, LOW, LOW, LOW, LOW, LOW, HIGH},       // 0
  {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH},   // 1
  {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW},      // 2
  {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW},      // 3
  {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW},     // 4
  {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW},      // 5
  {LOW, HIGH, LOW, LOW, LOW, LOW, LOW},       // 6
  {LOW, LOW, LOW, HIGH, HIGH, LOW, HIGH},     // 7
  {LOW, LOW, LOW, LOW, LOW, LOW, LOW},        // 8
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW},       // 9
  {LOW, LOW, LOW, HIGH, LOW, LOW, LOW},       // A
  {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},      // E
  {LOW, HIGH, HIGH, HIGH, LOW, LOW, LOW},     // F
  {LOW, HIGH, LOW, LOW, LOW, LOW, HIGH},      // G
  {HIGH, LOW, LOW, HIGH, LOW, LOW, LOW},      // H
  {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},   // I
  {HIGH, HIGH, HIGH, LOW, LOW, LOW, HIGH},    // L
  {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH},  // _
  {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}  // null
};

Adafruit_MCP23X17 mcp1;
Adafruit_MCP23X17 mcp2;

// Constants
const int maxLives = 4;
const int bumperPoint = 5;
const int comboTime = 3000;
const int displayRemainTime = 3000;
const int increaseTime = 900;
const int maxCombo = 5;
const int fallThreshold = 300;

// Pins
const int startButton = 15;
const int indicatorLED[6] = {4, 5, 6, 7, 8, 9};
const int numLED = 6;
const int fallSensor = A0;
const int solenoid = 14;
const int bumper = 16;

// Highscore
int highScore = 0;

void setup() {
  // Setup buttons
  pinMode(startButton, INPUT_PULLUP);
  pinMode(fallSensor, INPUT);
  pinMode(solenoid, OUTPUT);
  pinMode(bumper, INPUT_PULLUP);

  // Setup LEDs
  for (int i = 0; i < numLED; i++) {
    pinMode(indicatorLED[i], OUTPUT);
  }

  // Setup 7-segment displays
  mcp1.begin_I2C(0x20);
  mcp2.begin_I2C(0x21);
  for (int i = 0; i < 16; i++) {
    mcp1.pinMode(i, OUTPUT);
    mcp2.pinMode(i, OUTPUT);
  }
  mcp1.digitalWrite(7, LOW);
  mcp1.digitalWrite(15, LOW);
  mcp2.digitalWrite(7, LOW);
  mcp2.digitalWrite(15, LOW);

  delay(comboTime);
}

void numToString(char str[], int num) {
  if (num >= 0) {
    for (int i = 3; i >= 0; i--) {
      str[i] = num % 10 + '0';
      num /= 10;
    }
  } else {
    for (int i = 0; i < 4; i++) {
      str[i] = '\0';
    }
  }
}

void display7Segment(char str[]) {
  // Convert a string to numbers
  int num[4];
  for (int i = 0; i < 4; i++) {
    if (str[i] >= '0' && str[i] <= '9') {
      num[i] = str[i] - '0';
    } else if (str[i] == 'A') {
      num[i] = 10;
    } else if (str[i] >= 'E' && str[i] <= 'I') {
      num[i] = str[i] - 'E' + 11;
    } else if (str[i] == 'L') {
      num[i] = 16;
    } else if (str[i] == '_') {
      num[i] = 17;
    } else {
      num[i] = 18;
    }
  }

  // Display characters
  for (int i = 0; i < 7; i++) {
    mcp1.digitalWrite(i, ledNum[num[0]][i]);
    mcp1.digitalWrite(i + 8, ledNum[num[1]][i]);
    mcp2.digitalWrite(i, ledNum[num[2]][i]);
    mcp2.digitalWrite(i + 8, ledNum[num[3]][i]);
  }
}

void game() {
  int lives = maxLives;
  int point = 5;
  int score = 0;
  bool isAlive = false;
  char remain[4], str[4];

  int isPushed = HIGH;
  int previous = HIGH;
  unsigned long currentTime = 0;
  unsigned long previousTime = 0;

  while (lives > 0 || isAlive == true) {
    if (isAlive == false) {
      // Display remaining balls
      for (int i = 0; i < 4; i++) {
        if (lives + i >= 4) {
          remain[i] = '0';
        } else {
          remain[i] = '_';
        }
      }

      // Display your life
      display7Segment("LIFE");
      delay(displayRemainTime / 3);
      display7Segment(remain);
      delay(displayRemainTime / 3 * 2);

      digitalWrite(solenoid, HIGH);
      display7Segment(" G0 ");
      delay(1000);
      digitalWrite(solenoid, LOW);

      isAlive = true;      
    } else {
      // Score
      // Count if the button has pushed
      currentTime = millis();
      isPushed = digitalRead(bumper);
      if (isPushed == LOW && previous == HIGH) {
        // Calculate points
        if (currentTime - previousTime < comboTime) {
          if (currentTime - previousTime < increaseTime) {
            point = min(point + 1, maxCombo);
          } else if (point <= 7) {
            point = min(point + 1, 7);
          }
        } else {
          point = bumperPoint;
        }

        score += point;

        previousTime = currentTime;
      }   

      int numberPh = 2;
      int phPin[] = {A0, A1};
      int thresholdsPh[] = {300, 300};
      int previousTimePh[] = {0, 0};
      int phPoint[] = {5, 5};

      // Detect phototransistors
      for (int i = 0; i < hoge; i++) {
        if (isPushed == LOW && previous == HIGH) {
          // Calculate points
          if (currentTime - previousTime < comboTime) {
            if (currentTime - previousTime < increaseTime) {
              point = min(point + 1, maxCombo);
            } else if (point <= 7) {
              point = min(point + 1, 7);
            }
          } else {
            point = bumperPoint;
          }

          score += point;

          previousTime = currentTime;
        }   
      }

      // Fall the ball
      if (analogRead(fallSensor) < fallThreshold) {
        isAlive = false;
        lives -= 1;

        display7Segment("FALL");
        delay(4000);
        numToString(str, -1);
      }
    }

    // Display remaining time
    for (int i = 1; i <= numLED; i++) {
      digitalWrite(indicatorLED[i - 1], (currentTime - previousTime < comboTime / numLED * i));
    }

    // Store a current condition
    previous = isPushed;

    // Prevent chatterings
    delay(1);

    // Display player's score
    numToString(str, score);
    display7Segment(str);
  }

  // Display final score(with flashing)
  numToString(str, score);
  for (int i = 0; i < 3; i++) {
    display7Segment(str);
    delay(1000);
    display7Segment("    ");
    delay(1000);
  }

  // Substitution highscore if score is larger than highscore
  if (highScore < score) {
    highScore = score;
    display7Segment("    ");
    delay(500);
    numToString(str, highScore);
    // Display highscore(with flashing)
    for (int i = 0; i < 3; i++) {
      display7Segment(str);
      delay(1000);
      display7Segment("    ");
      delay(1000);
    }
    
  }

}

char highStr[4];
unsigned int cTime, pTime;
void loop() {
  // Display highscore
  // DON'T use delay()
  cTime = millis();
  if ((cTime - pTime) % 2000 < 1000) {
    display7Segment("HIGH");
  } else {
    numToString(highStr, highScore);
    display7Segment(highStr);
  }
  

  // Game start
  if (digitalRead(startButton) == LOW) {
    game();
    pTime = millis();
  }
}
