# Arduino ESP8266 NodeMCU Motion sensor camera

---



## Quick links

---

- [ESPNOW for ESP8266 by Espressif - Non-OS SDK API Reference](https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf)
- [ESPNOW for ESP8266 by Espressif - API Setup Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html#_CPPv412esp_now_initv)

## Purpose of the project

---

This projects provides a person with steps to build and program Arduino ESP8266 NodeMCUs to detect motion, notify the user of the motion via a speaker and LED, track the time since alert, then allow the user to capture footage via an Arducam Mini 2MP Plus. The MCUs communicate via WiFi.

Motion is detected via the passive infrared sensor (PIR sensor). The user is alerted of motion for 14 seconds via sound from connected speaker and the illumination of an LED. During these 14 seconds the alert can be muted via pressing the push button. Time (HH:MM:SS) elapsed since alert is displayed on the LCD. The user now knows to capture footage.

## Contents

---

- [Arduino ESP8266 NodeMCU Motion sensor camera](#arduino-esp8266-nodemcu-motion-sensor-camera)

* [Quick links](#quick-links)

- [Purpose of the project](#purpose-of-the-project)
- [Parts list](#parts-list)
- [Circuit Schematics](#circuit-schematics)
  + [Circuit schematic 1 - Client](#circuit-schematic-1---client)
  + [Circuit schematic 2 - Server](#circuit-schematic-2---server)
  + [Circuit schematic 3 - Camera](#circuit-schematic-3---camera)
- [Setup](#setup)
- [How to use](#how-to-use)

## Parts list

---

| Item                                             | Quantity |
| :----------------------------------------------- | :------- |
| 400-point solderless breaboard                   | 3        |
| ESP8266 NodeMCU CP2102 ESP-12E Development Board | 3        |
| HD44780 I2c LCD display                          | 1        |
| HC-SR501 PIR Sensor                              | 1        |
| Arducam Mini 2MP Plus                            | 1        |
| CMS-40558N-L152 Speaker                          | 1        |
| EVQ-22707K Push button                           | 1        |
| 10K Resistor                                     | 1        |
| LED                                              | 1        |
| 5V 2.4A Power Supply with USB connection         | 1        |
| Micro-USB cable                                  | 3        |

## Circuit Schematics

### Circuit schematic 1 - Client

- Note: Diagram below does not show wiring connection for HD44780 I2c LCD. Please follow (LINK HERE) below to correctly wire HD44780 I2c LCD to NodeMCU.

![Circuit schematic for client](https://github.com/alexabram/Arduino-Motion_sensor_camera/blob/master/images/circuit_schematics/client.jpg?raw=true)

### Circuit schematic 2 - Server

![Circuit schematic for server](https://github.com/alexabram/Arduino-Motion_sensor_camera/blob/master/images/circuit_schematics/server.jpg?raw=true)

### Circuit schematic 3 - Camera

![Circuit schematic for Circuit schematic for camera](https://github.com/alexabram/Arduino-Motion_sensor_camera/blob/master/images/ArduCam-8266_pinout.jpg?raw=true)

## Setup

1. Follow [esp8266/Arduino GitHub repository](https://github.com/esp8266/arduino#installing-with-boards-manager) to setup Arduino on ESP8266.
2. Follow [ArduCam's ArduCAM Mini Cameras Tutorial](https://www.arducam.com/knowledge-base/mini-tutorial/) to setup ArduCAM
3. For each board, open Tools and set Upload Speed to 115200.
4. Upload sketch in `master/setup/Get_8266_MACaddr/Get_8266_MACaddr.ino` to board used as the client.
   1. Open serial monitor and copy the printed MAC address.
   2. In `master/src/sever/server.ino`, replace variable `client_mac_addr` with MAC address from previous step.
5. On Arduino IDE, open Tools > Manage Libraries menu and install the LiquidCrystal I2C library by Marco Scwartz.

## How to use

1. PIR sensor detects motion. (server)
2. User is alerted via sound from speaker and light from LED. Timer begins counting on LCD. (client)
3. User connects to WiFi SSID arducam_esp8266. Open `Library/Arduino15/packages/ArduCAM_ESP8266_UNO/hardware/ArduCAM_ESP8266_UNO/2.2.4/libraries/ArduCAM/examples/ESP8266/ArduCAM_ESP8266_UNO_Capture/html_2640/index.html` in browser to capture footage via camera.'

