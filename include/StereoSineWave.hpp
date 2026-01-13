#include "Config.hpp"
#include "Delay.hpp"
#include "Knob.hpp"
#include "effect_envelope.h"

#ifdef SSW

#define VOLUME_SAMPLES 5

#pragma once

#include <Audio.h>

namespace ssw {

extern AudioControlSGTL5000 sgtl5000_1;
extern AudioOutputI2S i2s1;

extern AudioSynthWaveformSine sound;

extern Delay gigaDelay;
extern AudioEffectEnvelope envelope;

extern AudioConnection connect1;
extern AudioConnection connect5;
extern AudioConnection connect6;

extern Knob volumeKnob;

void setup();
void loop();

} // namespace ssw

#endif
