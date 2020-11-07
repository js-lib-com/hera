#ifndef __HERA_E2PROM
#define __HERA_E2PROM

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_BUFFER_SIZE 32
#define EEPROM_STR_BUFFER_SIZE 100
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

    static void read(String& string) {
      EEPROM.begin(EEPROM_STR_BUFFER_SIZE);
      for (unsigned char addr = 0; addr < EEPROM_STR_BUFFER_SIZE; ++addr) {
        char c = EEPROM.read(addr);
        if (c == 0) {
          break;
        }
        string += c;
      }
      EEPROM.end();
    }

    static void write(const String& string) {
      EEPROM.begin(EEPROM_STR_BUFFER_SIZE);
      unsigned char addr = 0;
      unsigned char length = min(string.length(), ((unsigned int)EEPROM_STR_BUFFER_SIZE - 1));
      for (; addr < length; ++addr) {
        EEPROM.write(addr, string.charAt(addr));
      }
      EEPROM.write(addr, 0);
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

