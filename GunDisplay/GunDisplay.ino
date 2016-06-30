/*
  GunDisplay

  This displays the 2 7-segment display of the Halo gun ammo.
 */

#include <SPI.h>
#include "Adafruit_BLE_UART.h"
#include <Thread.h>
#include "AmmoDisplay.h"

#define HIGH 0x1
#define LOW  0x0

/******** BLUETOOTH *********
 Connect CLK/MISO/MOSI to hardware SPI
 e.g. On UNO & compatible: CLK = 13, MISO = 12, MOSI = 11
*/
#define ADAFRUITBLE_REQ 3
#define ADAFRUITBLE_RDY 2     // This should be an interrupt pin, on Uno thats #2 or #3
#define ADAFRUITBLE_RST 4

Adafruit_BLE_UART BTLEserial = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);
Thread btSendThread = Thread();
Thread btReceiveThread = Thread();

const char RELOAD_COMMAND[] = "Reload";
const int RELOAD_COMMAND_LENGTH = 6;
const int MAX_AMMO = 30;

AmmoDisplay display(MAX_AMMO);

int pastButtonState = 0;     // previous state of the button

const int BUTTON_PIN = 41; //5
const int SWITCH = 42; //8

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.

  display.init();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
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

void updateBluetoothAmmo() {
  if (btSendThread.shouldRun()) {
    aci_evt_opcode_t status = BTLEserial.getState();
    if (status == ACI_EVT_CONNECTED) {
          btSendThread.run();
    }
  }
}

void reload() {
  display.reload();
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
  char strlength = display.getCurrentAmmo() >= 10 ? 3 : 2;
  char str[strlength];
  sprintf(str, "%d", display.getCurrentAmmo());
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
    display.reload();
  }
}

// the loop function runs over and over again forever
void loop() {
  // Reload after a second if we're out of ammo
  if (display.getCurrentAmmo() == 0) {
    delay(1000);
    display.reload();
  }

  // Check the BT connection for any incoming data
  BTLEserial.pollACI();
  if (btReceiveThread.shouldRun()) {
    aci_evt_opcode_t status = BTLEserial.getState();
    if (status == ACI_EVT_CONNECTED) {
          btReceiveThread.run();
    }
  }

  display.turnOffDisplay();
  bool switchStatus = digitalRead(SWITCH);
  int buttonState = digitalRead(BUTTON_PIN);
  bool sendBluetoothUpdate = false;

  // //Single Shot Display
  if (switchStatus) {
    if (buttonState != pastButtonState) {
      if (!buttonState == HIGH) {
        Serial.println(display.getCurrentAmmo());
        bool ammoChanged = display.decrementAmmo();
        if (ammoChanged) {
          sendBluetoothUpdate = true;
        }
      }
    }
    pastButtonState = buttonState;
  } else {   //Machine Gun Display
    if (buttonState == LOW) {
      Serial.println(display.getCurrentAmmo());
      bool ammoChanged = display.decrementAmmo();
      if (ammoChanged) {
        sendBluetoothUpdate = true;
      }
    }
  }

  //Alternate between the 2 7-segment displays
  display.changeDisplaySide();
  display.refresh();

  if (sendBluetoothUpdate) {
    updateBluetoothAmmo();
  }

  delay(5);
}
