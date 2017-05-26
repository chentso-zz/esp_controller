# ESP-Controller

ESP8266-Controller is a barebones Arduino firmware written for the ESP8266 WiFi module general IoT applications. The firmware connects to a local WiFi network using SSID/password saved in the EEPROM and hosts a RESTful web server to enable and disable its GPIO pins. 

By default no SSID/password is saved in EEPROM so the esp-controller boots up in Access Point mode with the SSID name esp-{ChipID}. Connect to this network via the password 'admin0000' and access the web control panel at the address "http://192.168.4.1". Input your SSID/password and press save to restart the esp-controller in WiFi client mode. If the esp-controller fails to connect to the WiFi specified, it will reboot itself into Access Point mode again until a correct WLAN network is connected. 

### Version 0.11