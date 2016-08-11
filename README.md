# Halo-Gun

This repository contains Arduino code that controls the the Halo-Gun including:
- Displaying the current ammo inside the gun using a 14-segment display.
- Changing between single shot and multi shot firing mode using a switch.
- allowing for a bluetooth enabled device to connect to the gun.
- reloading the gun through the reload command over bluetooth or when the gun runs out of ammo.
- sending the current ammo to a bluetooth connected device.


## Setup
1. Download the [Adafruit BLE UART Arduino library](https://github.com/adafruit/Adafruit_nRF8001/archive/master.zip)
2. Add it to your local Arduino library folder ([instructions here](https://www.arduino.cc/en/Guide/Libraries))
3. Compile through the Arduino IDE
