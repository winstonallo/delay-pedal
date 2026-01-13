#pragma once

#ifndef KNOB_SAMPLES
#define KNOB_SAMPLES 5
#endif

#include <Audio.h>
#include <cstdint>

class Knob {
  private:
    uint8_t _pin;
    uint32_t _readInterval;
    uint16_t _samples[KNOB_SAMPLES];
    uint8_t _curr;
    uint16_t _sum;
    bool _filled;
    IntervalTimer _timer;
    static void (*_updateCallback)();

    uint16_t _readFromPin();

  public:
    Knob(uint8_t pin, uint32_t readInterval);

    Knob() = delete;

    void begin(void (*updateCallback)());

    uint8_t getPin() const;

    float32_t update();

    float32_t _getCurrentVolume() const;
};
