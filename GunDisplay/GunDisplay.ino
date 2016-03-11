/*
  GunDisplay

  This displays the 2 7-segment display of the Halo gun ammo.
 */

#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Thread.h>

#define HIGH 0x1
#define LOW  0x0

/******** BLUETOOTH *********
 Connect CLK/MISO/MOSI to hardware SPI
 e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
*/
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 9

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
Thread btSendThread = Thread();
Thread btReceiveThread = Thread();

const char RELOAD_COMMAND[] = "Reload";
const int RELOAD_COMMAND_LENGTH = 6;
const int MAX_AMMO = 30;
int amountOfAmmo = MAX_AMMO;

int pastButtonState = 0;     // previous state of the button

boolean showLeftNumber = true; //If false, showRightNumber

int SPEED = 9; //How fast the numbers deincrement when in machine gun mode
int timeLapse = 0; //A counter for the determing when to deincrement machine gun mode ammo

//Pins of the display
const int DIGIT_TOP_BAR = 14; //2
const int DIGIT_MIDDLE_VERTICAL_BARS = 15; //3
const int DIGIT_MIDDLE_HORIZONTAL_BARS = 16; //4
const int DIGIT_UPPER_RIGHT_VERTICAL_BAR = 17; //5
const int DIGIT_LOWER_RIGHT_VERTICAL_BAR = 18; //6
const int DIGIT_LOWER_LEFT_VERTICAL_BAR = 19; //7
const int DIGIT_UPPER_LEFT_VERTICAL_BAR = 3; //8
const int DIGIT_BOTTOM_BAR = 4; //9
const int BUTTON_PIN = 5; //10
const int LEFT_NUMBER_DISPLAY = 6; //11
const int RIGHT_NUMBER_DISPLAY = 7; //12
const int SWITCH = 8; //13

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
  while(!Serial); // Wait for serial init
  BTLEserial.begin();
  btSendThread.onRun(bluetoothSendThreadMain);
  // We will only run this thread when the ammo is decremented,
  // so we don't want any other internal interfering
  btSendThread.setInterval(0);
  btReceiveThread.onRun(bluetoothReceiveThreadMain);
  btReceiveThread.setInterval(100);
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

bool deincrementAmmo() {
  if (amountOfAmmo > 0 && timeLapse >= SPEED) {
    turnOffDisplay();
    amountOfAmmo--;
    timeLapse = 0;
    return true;
  }
  return false;
}

void updateBluetoothAmmo() {
  if (btSendThread.shouldRun()) {
    aci_evt_opcode_t status = BTLEserial.getState();
    if (status == ACI_EVT_CONNECTED) {
          btSendThread.run();
    }
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

void reload() {
  amountOfAmmo = 30;
  updateBluetoothAmmo();
}

char * readBluetooth(Adafruit_BLE_UART BTLEserial) {
  // Lets see if there's any data for us!
  int bytesavailable = BTLEserial.available();
  if (bytesavailable) {
    Serial.print("* "); Serial.print(bytesavailable); Serial.println(F(" bytes available from BTLE"));
  }

  char * buffer = (char *) calloc(bytesavailable, sizeof(char));
  for (int i = 0; i < bytesavailable; i++) {
    buffer[i] = BTLEserial.read();
  }

  return buffer;
}

void sendBluetooth(Adafruit_BLE_UART BTLEserial, uint8_t * sendbuffer, char sendbuffersize) {
  //Serial.print(F("\n* Sending -> \"")); Serial.print((char *)sendbuffer); Serial.println("\"");
  BTLEserial.write(sendbuffer, sendbuffersize);
}

void sendAmmoOverBluetooth() {
  char strlength = amountOfAmmo >= 10 ? 3 : 2;
  char str[strlength];
  sprintf(str, "%d", amountOfAmmo);
  sendBluetooth(BTLEserial, (uint8_t*)str, strlength);
}

bool checkForReloadCommand() {
  if (BTLEserial.available() > 0) {
    char * message = readBluetooth(BTLEserial);
    int result = strncmp(message, RELOAD_COMMAND, RELOAD_COMMAND_LENGTH);
    if (result == 0) {
      Serial.println("Reload");
      return true;
    }
  }
  return false;
}

// Called every time btSendThread is ran
void bluetoothSendThreadMain() {
  sendAmmoOverBluetooth();
}

// Called every time btReceiveThread is ran
void bluetoothReceiveThreadMain() {
  bool needsReload = checkForReloadCommand();
  if (needsReload) {
    reload();
  }
}

// the loop function runs over and over again forever
void loop() {
  //  testBarsAreWorking();
  //  testNumbers();
  // Reload after a second if we're out of ammo
  if (amountOfAmmo == 0) {
    delay(1000);
    reload();
  }

  // Check the BT connection for any incoming data
  BTLEserial.pollACI();
  if (btReceiveThread.shouldRun()) {
    aci_evt_opcode_t status = BTLEserial.getState();
    if (status == ACI_EVT_CONNECTED) {
          btReceiveThread.run();
    }
  }

  turnOffDisplay();
  bool switchStatus = digitalRead(SWITCH);
  int buttonState = digitalRead(BUTTON_PIN);
  bool sendBluetoothUpdate = false;

  // //Single Shot Display
  if (switchStatus) {
    if (buttonState != pastButtonState) {
      if (!buttonState == HIGH) {
        Serial.println(amountOfAmmo);
        bool ammoChanged = deincrementAmmo();
        if (ammoChanged) {
          sendBluetoothUpdate = true;
        }
      }
    }
    pastButtonState = buttonState;
  } else {   //Machine Gun Display
    if (buttonState == LOW) {
      Serial.println(amountOfAmmo);
      bool ammoChanged = deincrementAmmo();
      if (ammoChanged) {
        sendBluetoothUpdate = true;
      }
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

  if (sendBluetoothUpdate) {
    updateBluetoothAmmo();
  }

  delay(5);
}
