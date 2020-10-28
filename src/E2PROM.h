#ifndef __HERA_E2PROM
#define __HERA_E2PROM

#include <EEPROM.h>

#define EEPROM_BUFFER_SIZE 32
#define NO_EEPROM (byte)255

class E2PROM {
  public:
    static unsigned char read(unsigned char addr) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      unsigned char value = EEPROM.read(addr);
      EEPROM.end();
      return value;
    }

    static void write(unsigned char addr, unsigned char value) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.write(addr, value);
      EEPROM.end();
    }

    static void get(unsigned char addr, float& reference) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.get(addr, reference);
      EEPROM.end();
    }

    static void put(unsigned char addr, float value) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.put(addr, value);
      EEPROM.end();
    }
};

#endif

