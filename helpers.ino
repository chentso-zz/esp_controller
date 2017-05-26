/** Convert integer to hexadecimal
 *  
 */
String int2hex(unsigned long v){
  /* Get the number of characters needed. (+1 for NUL terminator) */
  int n = (int) (log(v) / log(16) + 1) + 1;
  
  /* Create a char-string and NUL terminate the string */
  char h[n];
  h[n-1] = '\0';
  
  /* Start from the last character */
  int idx = n - 2;
  
  while(idx >= 0){
    /* Get only the least significant 4 bits */
    byte val = v & 0xF;

    /* Bit-shift number 4-bits to the right */
    v >>= 4;
    
    /* Determine the hexadecimal alphanumeric character  */
    h[idx--] = (val > 9) ? val - 10 + 'A' : val + '0';
  }
  
  return String(h);
}

/** Helper function to debug variables
 *  
 */
void debug(String l, String v){
  Serial.print(l + ": ");
  Serial.println(v);
}

/** Helper function to debug variables
 *  
 */
void debug(String l, char v[]){
  debug(l, String(v));
}

/** Helper function to debug variables
 *  
 */
void debug(String l, int v){
  debug(l, String(v));
}

void spacer(){
  Serial.println();
}

String get_argument(String http_arg, String key){
  String arg = "";
  int start = http_arg.indexOf(key+"=");
  
  /* If key isn't found, then return nothing */
  if (start == -1) return "";

  for(int i = start + key.length(); i < http_arg.length(); i++) {
    if (http_arg.charAt(i) == '&') {
      arg = http_arg.substring(start, i);
      break;
    }

    if (i == http_arg.length() - 1) {
      arg = http_arg.substring(start, i+1);
    }
  }

  return arg.substring(arg.indexOf("=")+1, arg.length());
}


String toString(IPAddress ip){
  return String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
}

