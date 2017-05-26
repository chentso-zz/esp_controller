int maxWaitTime = 10;
boolean is_wifi_mode = false;

void connect_to_wifi(){
  delay(10);
  String ssid = getSSID();
  String pass = getPass();

  ssid.replace("+", " ");
  /*  Connect to WiFi network  */
  Serial.println("\n\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Use the saved ssid and password */
  WiFi.begin(ssid.c_str(), pass.c_str());

  int time = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    
    // Check for timeout
    if (time++ == maxWaitTime) break;
  }

  if (WiFi.status() != WL_CONNECTED) {
    spacer();
    Serial.println("Could not connect to the wifi.. restarting in AP Mode");
    eeprom_write_startmode(MODE_AP);
    ESP.restart();
    return;
  } else if (WiFi.status() == WL_CONNECTED) {
    is_wifi_mode = true;
    server.begin();
    spacer();
    Serial.println("Server started!");
    Serial.print("Server IP Address: ");
    Serial.println(WiFi.localIP());

/* For SMS broadcast of ip address */
//    WiFiClient client;
//    const int httpPort = 5000;
//    const char* host = "192.168.0.110";
//    if (!client.connect(host, httpPort)) {
//      Serial.println("connection failed");
//      return;
//    }
//
//    IPAddress wifi_ip = WiFi.localIP();
//    String url = "/sms/";
//    url += "{phone number}";
//    url += toString(wifi_ip);
//
//    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
//                 "Host: " + host + "\r\n" + 
//                 "Connection: close\r\n\r\n");
    delay(10);
  }

}

void wifi_mode_loop(){
  if (!is_wifi_mode) return;

  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New client connected");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else if (req.indexOf("/apmode") != -1) {
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
    s =+ "<br>Restarting in access point mode, please reconnect to AP: " + get_ap_name() + "  when it is available. </br>";    

    // Set mode to AP and restart device
    eeprom_write_startmode(MODE_AP);
    ESP.restart(); 
  } else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
}

