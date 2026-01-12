#include "Config.hpp"
#include "GigaDelay.hpp"

#ifdef SSW

#define VOLUME_SAMPLES 5

#pragma once

#include <Audio.h>

namespace ssw {

extern AudioControlSGTL5000 sgtl5000_1;
extern AudioOutputI2S i2s1;
extern AudioSynthWaveformSine sound1;
extern AudioSynthWaveformSine sound2;
extern GigaDelay envelope1;
extern GigaDelay envelope2;
extern AudioConnection connect1;
extern AudioConnection connect2;
extern AudioConnection connect3;
extern AudioConnection connect4;
extern AudioConnection connect5;
extern AudioConnection connect6;

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

void setup();
void loop();
void updateVolume();

} // namespace ssw

#endif
