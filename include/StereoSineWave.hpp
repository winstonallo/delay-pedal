#pragma once

#include <Audio.h>

namespace ssw {

extern AudioControlSGTL5000 sgtl5000_1;
extern AudioOutputI2S i2s1;
extern AudioSynthWaveformSine sound1;
extern AudioSynthWaveformSine sound2;
extern AudioEffectEnvelope envelope1;
extern AudioEffectEnvelope envelope2;
extern AudioConnection connect1;
extern AudioConnection connect2;
extern AudioConnection connect3;
extern AudioConnection connect4;
extern AudioConnection connect5;
extern AudioConnection connect6;

void setup();
void loop();

} // namespace ssw
