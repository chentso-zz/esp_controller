#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Esp.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>

WiFiServer server(80);

const int MODE_NORMAL = 0;
const int MODE_AP = 1;
const int MODE_FACTORY_RESET = 2;

const char *versionNumber = "0.11";

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n");
  Serial.println("esp-controller version: " + String(versionNumber));
  Serial.println("-------------------------------------------------");

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  eeprom_get_all();
  int startMode = (getSSID() == "") ? MODE_AP : eeprom_read_startmode();

  spacer();

  switch (startMode){
    case MODE_NORMAL:
      Serial.println("Starting in normal client wifi mode..");
      // normal wifi client mode
      connect_to_wifi();
      break;
    case MODE_AP:
      Serial.println("Starting in Access Point mode..");
      eeprom_write_startmode(MODE_NORMAL);
      put_into_ap_mode();
      break;
    case MODE_FACTORY_RESET:
      Serial.println("Performing a factory reset..");
      eeprom_clear();
      delay(1000);
      ESP.restart();
      break;
    default:
      Serial.println("Starting in default mode");
      // normal wifi client mode
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ap_mode_loop();

  wifi_mode_loop();
}


