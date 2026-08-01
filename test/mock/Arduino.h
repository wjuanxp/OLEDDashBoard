// Mock Arduino.h for host-side compile verification only.
#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef uint8_t byte;
typedef uint16_t word;

#define HIGH 0x1
#define LOW 0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define A0 0x40
#define A1 0x41
#define A2 0x42
#define A3 0x43
#define A4 0x44
#define A5 0x45

class MockSerial {
public:
    void begin(unsigned long) {}
    void print(const char*) {}
    void print(int) {}
    void print(unsigned long) {}
    void print(float) {}
    void println() {}
    void println(const char* s) { print(s); }
    void println(int v) { print(v); }
    void println(float v) { print(v); }
};
extern MockSerial Serial;

#define PROGMEM
#define pgm_read_byte(addr) (*(const uint8_t*)(addr))
#define pgm_read_word(addr) (*(const uint16_t*)(addr))
#define pgm_read_ptr(addr) (*(void* const*)(addr))
#define pgm_read_byte_near(addr) (*(const uint8_t*)(addr))

#define F(s) (s)

static inline void delay(unsigned long) {}
static inline void yield() {}
static inline unsigned long millis() { return 0; }
static inline void pinMode(uint8_t, uint8_t) {}
static inline void digitalWrite(uint8_t, uint8_t) {}
static inline int digitalRead(uint8_t) { return 0; }
static inline int analogRead(uint8_t) { return 0; }

#endif  // MOCK_ARDUINO_H
