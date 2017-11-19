
/*
  GunDisplay

  This displays the 2 7-segment display of the Halo gun ammo.
 */

#include <CurieBLE.h>
#include "AmmoDisplay.h"

#define HIGH 0x1
#define LOW  0x0

BLEPeripheral blePeripheral; // create peripheral instance

BLEService gunService("19B10000-E8F2-537E-4F6C-D104768A1214");
// create switch characteristic and allow remote device to read and write
const int MAX_AMMO = 30;
BLEUnsignedIntCharacteristic ammo("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead);

const int NO_COMMAND = 0;
const int RELOAD_COMMAND = 1;
// TODO: Add other commands for assault rifle (single shot and full auto)
BLEUnsignedIntCharacteristic command("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

AmmoDisplay display(MAX_AMMO);

int pastButtonState = 0;     // previous state of the button
bool firingMode = true;

const int BUTTON_PIN = 41; //5
const int SWITCH = 42; //8

// the setup function runs once when you press reset or power the board
void setup() {
  display.init();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SWITCH, INPUT_PULLUP);

  Serial.begin(9600);
  while(!Serial); // Wait for serial init

  blePeripheral.setAdvertisedServiceUuid(gunService.uuid());
  
  blePeripheral.addAttribute(gunService);
  blePeripheral.addAttribute(ammo);
  blePeripheral.addAttribute(command);

  // TODO: Change max ammo for pistol
  ammo.setEventHandler(BLERead, updateAmmo);
  ammo.setValue(MAX_AMMO);

  command.setEventHandler(BLEWritten, handleCommand);
  command.setValue(0);
  
  blePeripheral.begin();
}

void reload() {
  display.reload();
}

void updateAmmo(BLECentral& central, BLEUnsignedIntCharacteristic& ammo) {
  ammo.setValue(display.getCurrentAmmo());
}

void handleCommand(BLECentral& central, BLEUnsignedIntCharacteristic& command) {
  // TODO: handle different firing modes by changing firingMode boolean
  switch(command.value()) {
    case NO_COMMAND:
      break;
    case RELOAD_COMMAND:
      reload();
      break;
    default:
      break;
  }
}

// delay for a given amount of time to avoid running on the main loop
// while still being able to do work in the background like refresh the
// 14-segment display
void wait(int durationInMilliseconds) {
    int currentTimeThatHasBeenWaited = 0;
    const int WAIT_INCREMENT = 5;

    while (currentTimeThatHasBeenWaited < durationInMilliseconds) {
        delay(WAIT_INCREMENT);
        display.changeDisplaySide();
        currentTimeThatHasBeenWaited += WAIT_INCREMENT;
    }
}

bool getFiringModeStatus() {
  //NOTE: use digitalRead(SWITCH) to check physical switch mode
  //TODO: use both physical switch and bluetooth command to modify firing mode
  return firingMode;
}

// the loop function runs over and over again forever
void loop() {
  // Reload after a second if we're out of ammo
  if (display.getCurrentAmmo() == 0) {
    wait(1000);
    reload();
  }

  display.turnOffDisplay();
  bool switchStatus = getFiringModeStatus();
  int buttonState = digitalRead(BUTTON_PIN);

  // //Single Shot Display
  if (switchStatus) {
    if (buttonState != pastButtonState) {
      if (!buttonState == HIGH) {
        Serial.println(display.getCurrentAmmo());
        display.decrementAmmo();
      }
    }
    pastButtonState = buttonState;
  } else {   //Machine Gun Display
    if (buttonState == LOW) {
      Serial.println(display.getCurrentAmmo());
      display.decrementAmmo();
    }
  }

  //Alternate between the 2 7-segment displays
  display.changeDisplaySide();
  display.refresh();

  delay(5);
}
