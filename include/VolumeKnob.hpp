#pragma once

#ifndef VOLUME_SAMPLES
#define VOLUME_SAMPLES 5
#endif

#include <Audio.h>
#include <cstdint>

class VolumeKnob {
  private:
    uint8_t _pin;
    uint32_t _readInterval;
    uint16_t _samples[VOLUME_SAMPLES];
    uint8_t _curr;
    uint16_t _sum;
    bool _filled;
    IntervalTimer _timer;
    static void (*_updateCallback)();

    uint16_t _readFromPin();

  public:
    VolumeKnob(uint8_t pin, uint32_t readInterval);

    VolumeKnob() = delete;

    void begin(void (*updateCallback)());

    const uint8_t getPin() const;

    const float32_t update();

    const float32_t _getCurrentVolume() const;
};
