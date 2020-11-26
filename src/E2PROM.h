#ifndef __HERA_E2PROM
#define __HERA_E2PROM

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_BUFFER_SIZE (byte)255
#define EEPROM_STR_BUFFER_SIZE (byte)100
#define EEPROM_ADDR_OFFSET EEPROM_STR_BUFFER_SIZE + (byte)1
#define NO_EEPROM (byte)255

class E2PROM {
  public:
    static byte read(byte addr) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      byte value = EEPROM.read(EEPROM_ADDR_OFFSET + addr);
      EEPROM.end();
      return value;
    }

    static void write(byte addr, byte value) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.write(EEPROM_ADDR_OFFSET + addr, value);
      EEPROM.end();
    }

    static void get(byte addr, float& reference) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.get(EEPROM_ADDR_OFFSET + addr, reference);
      if (isnan(reference)) {
        reference = 0;
      }
      EEPROM.end();
    }

    static void put(byte addr, float value) {
      EEPROM.begin(EEPROM_BUFFER_SIZE);
      EEPROM.put(EEPROM_ADDR_OFFSET + addr, value);
      EEPROM.end();
    }

    static void read(String& string) {
      EEPROM.begin(EEPROM_STR_BUFFER_SIZE);
      for (byte addr = 0; addr < EEPROM_STR_BUFFER_SIZE; ++addr) {
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
      byte addr = 0;
      byte length = min(string.length(), ((unsigned int)EEPROM_STR_BUFFER_SIZE - 1));
      for (; addr < length; ++addr) {
        EEPROM.write(addr, string.charAt(addr));
      }
      EEPROM.write(addr, 0);
      EEPROM.end();
    }
};

#endif

