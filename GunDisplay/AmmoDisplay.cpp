#include "AmmoDisplay.h"

AmmoDisplay::AmmoDisplay(int maxAmmo) : MAX_AMMO(maxAmmo), currentAmmo(maxAmmo) {


}

void AmmoDisplay::init() {
    pinMode(DIGIT_TOP_BAR, OUTPUT);
    pinMode(DIGIT_BOTTOM_BAR, OUTPUT);
    pinMode(DIGIT_MIDDLE_VERTICAL_BARS, OUTPUT);
    pinMode(DIGIT_MIDDLE_HORIZONTAL_BARS, OUTPUT);
    pinMode(DIGIT_UPPER_RIGHT_VERTICAL_BAR, OUTPUT);
    pinMode(DIGIT_LOWER_RIGHT_VERTICAL_BAR, OUTPUT);
    pinMode(DIGIT_UPPER_LEFT_VERTICAL_BAR, OUTPUT);
    pinMode(DIGIT_LOWER_LEFT_VERTICAL_BAR, OUTPUT);
    pinMode(LEFT_NUMBER_DISPLAY, OUTPUT);
    pinMode(RIGHT_NUMBER_DISPLAY, OUTPUT);
}

int AmmoDisplay::getCurrentAmmo() {
    return currentAmmo;
}

void AmmoDisplay::reload() {
    currentAmmo = MAX_AMMO;
}

bool AmmoDisplay::decrementAmmo() {
  if (currentAmmo > 0 && timeLapse >= SPEED) {
    turnOffDisplay();
    currentAmmo--;
    timeLapse = 0;
    return true;
  }
  return false;
}

void AmmoDisplay::refresh() {
  timeLapse++;
  Serial.println(currentAmmo);
  int tensCase = currentAmmo / 10;
  int onesCase = currentAmmo % 10;
  if (showLeftNumber) {
    displayNumber(true, tensCase);
  } else {
    displayNumber(true, onesCase);
  }
}

void AmmoDisplay::changeDisplaySide() {
    if (showLeftNumber) {
      digitalWrite(LEFT_NUMBER_DISPLAY, HIGH);
      digitalWrite(RIGHT_NUMBER_DISPLAY, LOW);
      showLeftNumber = false;
    } else {
      digitalWrite(LEFT_NUMBER_DISPLAY, LOW);
      digitalWrite(RIGHT_NUMBER_DISPLAY, HIGH);
      showLeftNumber = true;
    }
}

//turns on and off led bars
void AmmoDisplay::displayLEDBar(int pinNumber, boolean isOn) {
  if (isOn == true) {
    digitalWrite(pinNumber, HIGH);
  } else {
    digitalWrite(pinNumber, LOW);
  }
}

int AmmoDisplay::displayNumberZero(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 0;
}

int AmmoDisplay::displayNumberOne(boolean isOn) {
  displayLEDBar(DIGIT_MIDDLE_VERTICAL_BARS, isOn);
  return 1;
}

int AmmoDisplay::displayNumberTwo(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  return 2;
}

int AmmoDisplay::displayNumberThree(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  return 3;
}

int AmmoDisplay::displayNumberFour(boolean isOn) {
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 4;
}

int AmmoDisplay::displayNumberFive(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 5;
}

int AmmoDisplay::displayNumberSix(boolean isOn) {
  displayNumberFive(isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  return 6;
}

int AmmoDisplay::displayNumberSeven(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  return 7;
}

int AmmoDisplay::displayNumberEight(boolean isOn) {
  displayNumberSix(isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  return 8;
}

int AmmoDisplay::displayNumberNine(boolean isOn) {
  displayNumberSeven(isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 9;
}

void AmmoDisplay::turnOffDisplay() {

  displayNumberZero(false);
  displayNumberOne(false);
  displayNumberTwo(false);
  displayNumberThree(false);
  displayNumberFour(false);
  displayNumberFive(false);
  displayNumberSix(false);
  displayNumberSeven(false);
  displayNumberEight(false);
  displayNumberNine(false);
}

void AmmoDisplay::displayNumber(boolean isOn, int digit) {
  switch (digit) {
    case 0:
      displayNumberZero(isOn);
      break;
    case 1:
      displayNumberOne(isOn);
      break;
    case 2:
      displayNumberTwo(isOn);
      break;
    case 3:
      displayNumberThree(isOn);
      break;
    case 4:
      displayNumberFour(isOn);
      break;
    case 5:
      displayNumberFive(isOn);
      break;
    case 6:
      displayNumberSix(isOn);
      break;
    case 7:
      displayNumberSeven(isOn);
      break;
    case 8:
      displayNumberEight(isOn);
      break;
    case 9:
      displayNumberNine(isOn);
      break;
    default:
      break;
  }
}
