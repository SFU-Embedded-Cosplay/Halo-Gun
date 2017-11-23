
#include <CurieBLE.h>


/*
  Magnum

  Handles button presses to change ammo and communicate with Android phone over Bluetooth LE
 */

#include <CurieBLE.h>

#define HIGH 0x1
#define LOW  0x0

BLEPeripheral blePeripheral; // create peripheral instance
int disconnectedCount = 0;

BLEService gunService("19B10000-E8F2-537E-4F6C-D104768A1214");

const int MAX_AMMO = 12;
BLECharCharacteristic ammo("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

const char NO_COMMAND = 0;
const char RELOAD_COMMAND = 1;
// TODO: Add other commands for assault rifle (single shot and full auto)
BLECharCharacteristic command("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

int pastButtonState = 0;     // previous state of the button
bool firingMode = true;

const int BUTTON_PIN = 7; //5
const int FIRE_LED_PIN = 2;
const int CONNECTION_LED_PIN = 3;
// On-board LED to be used for debugging
const int DEBUG_LED_PIN = 13;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(FIRE_LED_PIN, OUTPUT);
  pinMode(CONNECTION_LED_PIN, OUTPUT);
  pinMode(DEBUG_LED_PIN, OUTPUT);

  digitalWrite(DEBUG_LED_PIN, HIGH);
  digitalWrite(FIRE_LED_PIN, LOW);
  digitalWrite(CONNECTION_LED_PIN, LOW);

//  Serial.begin(9600);
//  while(!Serial); // Wait for serial init

  blePeripheral.setLocalName("Gun");
  blePeripheral.setAdvertisedServiceUuid(gunService.uuid());
  
  blePeripheral.addAttribute(gunService);
  blePeripheral.addAttribute(ammo);
  blePeripheral.addAttribute(command);

  // TODO: Change max ammo for pistol
  ammo.setValue(MAX_AMMO);

  command.setEventHandler(BLEWritten, handleCommand);
  command.setValue(0);
  
  blePeripheral.begin();
}

void reload() {
//  Serial.println("Reload");
  ammo.setValue(MAX_AMMO);
}

void decrementAmmo() {
  ammo.setValue(ammo.value() - 1);
}

void handleCommand(BLEDevice central, BLECharacteristic characteristic) {
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

bool getFiringModeStatus() {
  //TODO: use both physical switch and bluetooth command to modify firing mode
  return firingMode;
}

// the loop function runs over and over again forever
void loop() {
  // Reload after a second if we're out of ammo
  if (ammo.value() == 0) {
    delay(1000);
    reload();
  }

  int buttonState = digitalRead(BUTTON_PIN);

  if (buttonState != pastButtonState) {
    if (!buttonState == HIGH) {
//      Serial.println((int)ammo.value());
      decrementAmmo();
      digitalWrite(FIRE_LED_PIN, HIGH);
      delay(100);
      digitalWrite(FIRE_LED_PIN, LOW);
    }
  }
  pastButtonState = buttonState;

  BLECentral central = blePeripheral.central();
  if (central && central.connected()) {
    digitalWrite(CONNECTION_LED_PIN, HIGH);
    disconnectedCount = 0;
  }
  else {
//    Serial.println(disconnectedCount);
    if (disconnectedCount == 1000) {
      disconnectedCount = 0;
      blePeripheral.end();
      digitalWrite(CONNECTION_LED_PIN, HIGH);
      delay(100);
      digitalWrite(CONNECTION_LED_PIN, LOW);
      blePeripheral.begin();
//      Serial.println("Bluetooth reset");
    }
    disconnectedCount++;
    digitalWrite(CONNECTION_LED_PIN, LOW);
  }

  delay(5);
}
