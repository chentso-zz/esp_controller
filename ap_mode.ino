const char *ap_pass = "admin0000";
const char *SSID_KEY = "ssid";
const char *PASS_KEY = "pass";
const char *DEVNAME_KEY = "devName";

String ap_ssid = "esp";
boolean inApMode = false;

/** handle ap root connection
 *  
 */
void handle_ap_root(WiFiClient client) {
  String s = "<h1>ESP8266 Control Panel</h1>";
  s += "<p>Previous SSID: ";
  s += getSSID();
  s += "<p><form method='GET' action='setting'>";
  s += "<br><label>Device Name:</label><br><input name='" + String(DEVNAME_KEY) + "' value='" + getDeviceName() + "' length=32>";
  s += "<br><label>SSID:</label><br><input name='" + String(SSID_KEY) + "' length=32>";
  s += "<br><label>Password:</label><br><input name='" + String(PASS_KEY) + "' length=64>";
  s += "<br><br><input type='submit'></form>";
  client.print(s);
}



/** Put the esp8266 into Access Point mode
 *  
 */
void put_into_ap_mode(){
  String devName = getDeviceName();

  ap_ssid += "-";
  ap_ssid += (devName == "") ? int2hex(ESP.getChipId()) : devName;
  
  WiFi.softAP(ap_ssid.c_str(), ap_pass);

  Serial.println();
  Serial.println("AP mode started, connect to SSID: " + ap_ssid);
  
  server.begin();
  inApMode = true;
}



/** Handle client connections that's not in the root
 *  
 */
void handle_ap_connection(){
  // Ensure we are in AP mode
  if (!inApMode) return;

  WiFiClient client = server.available();
  String req = client.readStringUntil('\r');
  client.flush();

  req.replace(" HTTP/1.1", "");
  req.replace("GET ", "");
  if (req == "/") {
    eeprom_get_all();
    handle_ap_root(client);
  } if (req == "/reset/") {
    String s = "Resetting...";
    client.print(s); 
  } if (req.indexOf("/setting?") != -1) {
    
    String wifi_ssid = get_argument(req, String(SSID_KEY));
    String wifi_pass = get_argument(req, String(PASS_KEY));
    String devName = get_argument(req, String(DEVNAME_KEY));

    String s = devName + " / " + wifi_ssid + " / " + wifi_pass;
    client.print(s);
    save_ssid_pass(wifi_ssid, wifi_pass);
    save_devName(devName);
    eeprom_write_startmode(MODE_NORMAL);
    ESP.restart();
  }
}

String get_ap_name(){
  return ap_ssid + "-" + getDeviceName();
}

void ap_mode_loop(){
  handle_ap_connection();
}

