#include "Config.hpp"

#ifdef WVK

#pragma once

#include <Audio.h>

#define VOLUME_SAMPLES 5

// 4.1 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 11 // not actually used
#define SDCARD_SCK_PIN 13  // not actually used

namespace wvk {
extern AudioPlaySdWav playSdWav1;
extern AudioOutputI2S i2s1;
extern AudioConnection patchCord1;
extern AudioConnection patchCord2;
extern AudioControlSGTL5000 sgtl5000_1;

class Volume {
  private:
    uint8_t _pin;
    uint32_t _readInterval;
    uint16_t _samples[VOLUME_SAMPLES];
    uint8_t _curr;
    uint16_t _sum;
    bool _filled;
    IntervalTimer _timer;

  public:
    Volume(uint8_t pin, uint32_t readInterval);

    Volume() = delete;

    void begin();

    const uint8_t getPin() const;

    void addSample(uint16_t sample);

    const float32_t getVolume() const;
};

extern Volume volume;

void updateVolume();
void setup();
void loop();
} // namespace wvk

#endif