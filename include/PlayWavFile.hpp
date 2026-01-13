#include "Config.hpp"

#ifdef WVK

#pragma once

#include "Knob.hpp"
#include <Audio.h>

#define VOLUME_SAMPLES 5

// 4.1 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 11 // not actually used
#define SDCARD_SCK_PIN 13  // not actually used

namespace pwf {

extern AudioPlaySdWav playSdWav1;
extern AudioOutputI2S i2s1;
extern AudioConnection patchCord1;
extern AudioConnection patchCord2;
extern AudioControlSGTL5000 sgtl5000_1;

extern Knob volumeKnob;

void setup();
void loop();

} // namespace pwf

#endif
