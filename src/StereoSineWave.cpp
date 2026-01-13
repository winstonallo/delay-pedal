#include "Config.hpp"
#include "VolumeKnob.hpp"
#include "arm_math.h"
#include "core_pins.h"
#include "effect_envelope.h"

#ifdef SSW

#include "GigaDelay.hpp"
#include "StereoSineWave.hpp"
#include <Audio.h>

namespace ssw {

AudioControlSGTL5000 sgtl5000_1;
AudioOutputI2S i2s1;

AudioSynthWaveformSine sound;

AudioEffectEnvelope envelope;
GigaDelay gigaDelay;

AudioConnection connect1(sound, 0, envelope, 0);
AudioConnection connect5(envelope, 0, gigaDelay, 0);
AudioConnection connect6(gigaDelay, 0, i2s1, 0);

VolumeKnob volume(A14, 100000);
VolumeKnob delayKnob(A15, 100000);

void
setup() {
    Serial.begin(9600);
    AudioMemory(8);

    sound.amplitude(0.5);
    sound.frequency(300);
    sound.phase(180);

    envelope.attack(5);
    envelope.hold(50);

    envelope.decay(300);

    envelope.sustain(0);
    envelope.release(100);

    sgtl5000_1.enable();
    sgtl5000_1.volume(0.1);

    ssw::volume.begin([] {
        float32_t vol = volume.update();
        Serial.printf("Volume: %.0f%%\n", vol * (float32_t)100);
        ssw::sgtl5000_1.volume(vol);
    });

    ssw::delayKnob.begin([] {
        float32_t vol = delayKnob.update();
        float32_t time = vol * 4000.0;
        Serial.printf("Delay Time: %.3f ms\n", time);
        gigaDelay.setTime(time);
    });
}

void
loop() {
    envelope.noteOn();
    gigaDelay.noteOn();

    delay(2000);
}

} // namespace ssw

#endif
