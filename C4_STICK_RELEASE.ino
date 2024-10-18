#include <M5StickCPlus2.h>

#define BUZZER_PIN 2

int totalDuration = 50000;
int count = 61;
int minDelay = 50;
int fastBeepDelay = 100;
bool isBombActive = false;
bool isBombDefused = false;
unsigned long buttonPressStart = 0;
bool isButtonPressed = false;
int defuseTime = 5000;

void resetBombState() {
  isBombActive = false;
  isBombDefused = false;
  buttonPressStart = 0;
  isButtonPressed = false;
  StickCP2.Display.setTextColor(GREEN);
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.fillScreen(BLACK);
  StickCP2.Display.drawString("C4", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 2);
  StickCP2.Display.setTextSize(0.7);
  StickCP2.Display.drawString("By Shlappa", StickCP2.Display.width() / 2,
                              StickCP2.Display.height() / 1.35);
  StickCP2.Display.setTextSize(1);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  auto cfg = M5.config();
  StickCP2.begin(cfg);
  StickCP2.Display.setRotation(3);
  StickCP2.Display.setTextColor(GREEN);
  StickCP2.Display.setTextDatum(middle_center);
  StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
  StickCP2.Display.setTextSize(1);
  resetBombState();
}

void loop() {
  StickCP2.update();

  if (!isBombActive && StickCP2.BtnA.wasPressed()) {
    isBombActive = true;
    isBombDefused = false;
    int remainingTime = totalDuration;
    int initialDelay = totalDuration / count;

    for (int i = 0; i < count; i++) {
      StickCP2.update();

      if (StickCP2.BtnA.isPressed()) {
        if (!isButtonPressed) {
          buttonPressStart = millis();
          isButtonPressed = true;
        }

        if (millis() - buttonPressStart > defuseTime) {
          isBombDefused = true;
          StickCP2.Display.fillScreen(GREEN);
          StickCP2.Display.setCursor(10, 10);
          StickCP2.Display.setTextColor(BLUE);
          StickCP2.Display.setTextSize(1.5);
          StickCP2.Display.drawString("Defused!", StickCP2.Display.width() / 2,
                                      StickCP2.Display.height() / 2);
          noTone(BUZZER_PIN);
          delay(5000);
          resetBombState();
          return;
        }
      } else {
        isButtonPressed = false;
        buttonPressStart = 0;
      }

      if (isBombDefused) {
        break;
      }

      tone(BUZZER_PIN, 900, 200);
      delay(200);

      noTone(BUZZER_PIN);

      int timeDelay;

      if (remainingTime <= 5000 && i >= count - 5) {
        timeDelay = fastBeepDelay;
      } else {
        timeDelay = map(i, 0, count - 1, initialDelay, minDelay);
      }

      delay(timeDelay);

      remainingTime -= (timeDelay + 200);

      StickCP2.Display.fillScreen(BLACK);
      StickCP2.Display.setCursor(10, 10);
      StickCP2.Display.drawString(String(count - i), StickCP2.Display.width() / 2,
                                  StickCP2.Display.height() / 2);
    }

    if (!isBombDefused) {
      tone(BUZZER_PIN, 1000, 1000);
      delay(1000);
      tone(BUZZER_PIN, 700, 1000);
      delay(1000);
      noTone(BUZZER_PIN);
    }

    resetBombState();
  }
}
