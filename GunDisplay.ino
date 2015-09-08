/*
  GunDisplay

  This displays the 2 7-segment display of the Halo gun ammo.
 */
#define HIGH 0x1
#define LOW  0x0

const int MAX_AMMO = 30;
int amountOfAmmo = MAX_AMMO;

int pastButtonState = 0;     // previous state of the button

boolean showLeftNumber = true; //If false, showRightNumber

int SPEED = 9; //How fast the numbers deincrement when in machine gun mode
int timeLapse = 0; //A counter for the determing when to deincrement machine gun mode ammo

//Pins of the display
const int DIGIT_TOP_BAR = 2;
const int DIGIT_MIDDLE_VERTICAL_BARS = 3;
const int DIGIT_MIDDLE_HORIZONTAL_BARS = 4;
const int DIGIT_UPPER_RIGHT_VERTICAL_BAR = 5;
const int DIGIT_LOWER_RIGHT_VERTICAL_BAR = 6;
const int DIGIT_LOWER_LEFT_VERTICAL_BAR = 7;
const int DIGIT_UPPER_LEFT_VERTICAL_BAR = 8;
const int DIGIT_BOTTOM_BAR = 9;
const int BUTTON_PIN = 10;
const int LEFT_NUMBER_DISPLAY = 11;
const int RIGHT_NUMBER_DISPLAY = 12;
const int SWITCH = 13;

//turns on and off led bars 
void displayLEDBar(int pinNumber, boolean isOn) {
  if (isOn == true) {
    digitalWrite(pinNumber, HIGH);
  } else {
    digitalWrite(pinNumber, LOW);
  }
}

int displayNumberZero(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 0;
}

int displayNumberOne(boolean isOn) {
  displayLEDBar(DIGIT_MIDDLE_VERTICAL_BARS, isOn);
  return 1;
}

int displayNumberTwo(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  return 2;
}

int displayNumberThree(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  return 3;
}

int displayNumberFour(boolean isOn) {
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 4;
}

int displayNumberFive(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_BOTTOM_BAR, isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 5;
}

int displayNumberSix(boolean isOn) {
  displayNumberFive(isOn);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, isOn);
  return 6;
}

int displayNumberSeven(boolean isOn) {
  displayLEDBar(DIGIT_TOP_BAR, isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, isOn);
  return 7;
}

int displayNumberEight(boolean isOn) {
  displayNumberSix(isOn);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, isOn);
  return 8;
}

int displayNumberNine(boolean isOn) {
  displayNumberSeven(isOn);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, isOn);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, isOn);
  return 9;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(DIGIT_TOP_BAR, OUTPUT);
  pinMode(DIGIT_BOTTOM_BAR, OUTPUT);
  pinMode(DIGIT_MIDDLE_VERTICAL_BARS, OUTPUT);
  pinMode(DIGIT_MIDDLE_HORIZONTAL_BARS, OUTPUT);
  pinMode(DIGIT_UPPER_RIGHT_VERTICAL_BAR, OUTPUT);
  pinMode(DIGIT_LOWER_RIGHT_VERTICAL_BAR, OUTPUT);
  pinMode(DIGIT_UPPER_LEFT_VERTICAL_BAR, OUTPUT);
  pinMode(DIGIT_LOWER_LEFT_VERTICAL_BAR, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LEFT_NUMBER_DISPLAY, OUTPUT);
  pinMode(RIGHT_NUMBER_DISPLAY, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  Serial.begin(9600);
}

void testBarsAreWorking() {
  displayLEDBar(DIGIT_TOP_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_TOP_BAR, false);
  displayLEDBar(DIGIT_MIDDLE_VERTICAL_BARS, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_MIDDLE_VERTICAL_BARS, false);
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_MIDDLE_HORIZONTAL_BARS, false);
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_UPPER_RIGHT_VERTICAL_BAR, false);
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_LOWER_RIGHT_VERTICAL_BAR, false);
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_UPPER_LEFT_VERTICAL_BAR, false);
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_LOWER_LEFT_VERTICAL_BAR, false);
  displayLEDBar(DIGIT_BOTTOM_BAR, true);
  delay(1000);              // wait for a second
  displayLEDBar(DIGIT_BOTTOM_BAR, false);
}

void turnOffDisplay() {

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

void testNumbers() {
  int onesCase = amountOfAmmo % 10;
  displayNumber(false, onesCase);
  amountOfAmmo++;
  onesCase = amountOfAmmo % 10;
  displayNumber(true, onesCase);
  delay(1000);
}

void deincrementAmmo() {
  if (amountOfAmmo > 0 && timeLapse >= SPEED) {
    turnOffDisplay();
    amountOfAmmo--;
    timeLapse = 0;
  }
}

void displayAmmo() {
  timeLapse++;
  Serial.println(amountOfAmmo);
  int tensCase = amountOfAmmo / 10;
  int onesCase = amountOfAmmo % 10;
  if (showLeftNumber) {
    displayNumber(true, tensCase);
  } else {
    displayNumber(true, onesCase);
  }

}

void displayNumber(boolean isOn, int digit) {
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

// the loop function runs over and over again forever
void loop() {
  //  testBarsAreWorking();
  //  testNumbers();

  turnOffDisplay();
  bool switchStatus = digitalRead(SWITCH);
  int buttonState = digitalRead(BUTTON_PIN);

  // //Single Shot Display
  if (switchStatus) {
    if (buttonState != pastButtonState) {
      if (!buttonState == HIGH) {
        Serial.println(amountOfAmmo);
        deincrementAmmo();
      }
    }
    pastButtonState = buttonState;
  } else {   //Machine Gun Display
    if (buttonState == LOW) {
      Serial.println(amountOfAmmo);
      deincrementAmmo();
    }
  }

  //Alternate between the 2 7-segment displays
  if (showLeftNumber) {
    digitalWrite(LEFT_NUMBER_DISPLAY, HIGH);
    digitalWrite(RIGHT_NUMBER_DISPLAY, LOW);
    showLeftNumber = false;
  } else {
    digitalWrite(LEFT_NUMBER_DISPLAY, LOW);
    digitalWrite(RIGHT_NUMBER_DISPLAY, HIGH);
    showLeftNumber = true;
  }
  displayAmmo();
  delay(5);
}
