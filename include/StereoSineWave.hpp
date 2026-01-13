#include "Config.hpp"
#include "GigaDelay.hpp"
#include "VolumeKnob.hpp"

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

extern VolumeKnob volume;

void setup();
void loop();

} // namespace ssw

#endif
