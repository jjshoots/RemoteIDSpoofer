# RIDS - Remote ID Spoofer

**Disclaimer**: This repository and its code are intended for educational purposes only.

An ESP8266/NodeMCU Drone RemoteID Spoofer.
Built based on work done by [sxjack](https://github.com/sxjack/uav_electronic_ids) and [SpacehuhnTech](https://github.com/SpacehuhnTech/esp8266_deauther).
I stand on the shoulders of giants.

This spawns 16 different fake drones broadcasting RemoteID, with them all flying in random directions around a particular GPS location.

<img src="./images/proof.jpg"  width="600">

## Installation

1. You need the [Arduino IDE](https://www.arduino.cc/en/software).
2. In Arduino IDE, go to `File` > `Preferences`, then add this URL to the `Additional Boards Manager URLs`:
	- https://raw.githubusercontent.com/SpacehuhnTech/arduino/main/package_spacehuhn_index.json
3. Now go to `Tools` > `Boards` > `Boards Manager`, search `deauther` and install `Deauther ESP8266 Boards`.
4. Select your board at `Tools` > `Board` > and be sure it is at `Deauther ESP8266 Boards` (and not at `ESP8266 Modules`).
5. Plug in your device, I used a NodeMCU v3, and select its COM port at `Tools` > `Port`.
6. Press `upload`, or use Ctrl+U.
7. The device should start broadcasting RemoteID packets generated for random flying machines.

## To-Do List

1. Add GPS capability to automatically create IDs wherever the device is located.


