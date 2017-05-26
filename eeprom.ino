/* SSID Name (max-length: 32) */
String ssid;
int ssid_addr_start = 0;
int ssid_addr_end = 31;

/* Password (max-length: 64) */
String pass;
int pass_addr_start = 32;
int pass_addr_end = 95;

/* Device Name (max-length: 32) */
String deviceName;
int name_addr_start = 96;
int name_addr_end = 127;

/* Start Mode */
int start_mode_addr = 128;

const static int ep_size = 4096;
boolean is_eeprom_init = false;


/** Initialize and read the EEPROM into memory
 *  
 */
void eeprom_init(){
  EEPROM.begin(ep_size);
  is_eeprom_init = true;
}


/** Clear the entire EEPROM
 *  
 */
void eeprom_clear(){
  if (!is_eeprom_init) eeprom_init();
  for(int i = 0; i < ep_size; i++){
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}



/** Read the EEPROM from a start address to end address into a string
 *  
 */
String eeprom_read(int ad0, int ad1){
  
  /* If EEPROM hasn't been initialized, then initialize it */
  if (!is_eeprom_init) eeprom_init();
  
  char *buffer = (char*) malloc((ad1 - ad0 + 1) * sizeof(char));
  String result;
  
  /* index for the buffer */
  int idx = 0;

  /** Loop from start address to end address in EEPROM and copy 
   *  only valid characters to the buffer
   */
  for (int i = ad0; i <= ad1; i++) {
    char c = EEPROM.read(i);
    if (c >= 32 && c < 255) {
      buffer[idx++] =  c;
    } else {
      break;
    }
  }
  
  /* NUL terminate the string */
  buffer[idx] = '\0';

  /* Get the result and free buffer pointer */
  result = String(buffer);
  free(buffer);
  
  return result;
}


/** Write str to the EEPROM from ad0 to ad1
 *  
 */
void eeprom_write(int ad0, int ad1, String str){
  
  /* If EEPROM hasn't been initialized, then initialize it */
  if (!is_eeprom_init) eeprom_init();

  /* Write the string into bytes */
  int addr = ad0;
  int i = 0;
  while (i < str.length()){
    EEPROM.write(addr++, str.charAt(i++));
  }

  /* Clear the remaining set of allocated bytes */
  while (addr <= ad1) {
    EEPROM.write(addr++, 0);
  }

  EEPROM.commit();
}


/** Read all the relevant information from the EEPROM 
 *  
 */
void eeprom_get_all(){
  eeprom_get_ssid_pass();
  eeprom_get_device_name();
}

void eeprom_get_ssid_pass(){
  /* Read the wifi ssid/pass */
  ssid = eeprom_read(ssid_addr_start, ssid_addr_end);
  pass = eeprom_read(pass_addr_start, pass_addr_end);
  
  debug("ssid", ssid);
  debug("pass", pass);
}

void eeprom_get_device_name(){
  /* Get the device name */
  deviceName = eeprom_read(name_addr_start, name_addr_end);
  debug("device-name", deviceName);
}

/** Save the ssid and password into the EEPROM
 *  
 */
void save_ssid_pass(String ssid, String pass){
  ssid.replace("+", " ");
  eeprom_write(ssid_addr_start, ssid_addr_end, ssid);
  eeprom_write(pass_addr_start, pass_addr_end, pass);
}

/** Save the device name
 *  
 */
void save_devName(String devName){
  eeprom_write(name_addr_start, name_addr_end, devName);
}

String getSSID(){
  return ssid;
}

String getPass(){
  return pass;
}

String getDeviceName(){
  return deviceName;
}

byte eeprom_read_startmode(){
  /* If EEPROM hasn't been initialized, then initialize it */
  if (!is_eeprom_init) eeprom_init();
  byte mode = EEPROM.read(start_mode_addr);

  /* If for some reason the byte was not written, write normal mode */
  if (mode == 0xFF) eeprom_write_startmode(MODE_NORMAL);
  
  return EEPROM.read(start_mode_addr);
}

void eeprom_write_startmode(byte mode){
  /* If EEPROM hasn't been initialized, then initialize it */
  if (!is_eeprom_init) eeprom_init();
  EEPROM.write(start_mode_addr, mode);
  EEPROM.commit();
}


