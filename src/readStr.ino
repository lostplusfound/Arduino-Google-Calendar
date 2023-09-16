String readStr(int addr) {
  int i = addr;
  char readChar = EEPROM.read(i);
  String str;
  while(readChar != '\0') {
    str += readChar;
    readChar = EEPROM.read(++i);
  }
  return str;
}
void putStr(int addr, String &str) {
  for(int i = addr; i < str.length() + addr; i++) {
    EEPROM.write(i, str[i-addr]);
  }
  EEPROM.write(str.length() + addr, '\0');
  EEPROM.commit();
}
