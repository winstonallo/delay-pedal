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
        ssw::sgtl5000_1.volume(vol);
        // Serial.printf("Volume: %f\n", vol);
    });
    gigaDelay.setTime(1000);
    ssw::delayKnob.begin([] {
        float32_t vol = delayKnob.update();
        float32_t time = vol * 1000.0;
        Serial.printf("Normalized Value: %f, Time: %f\n", vol, time);
        gigaDelay.setTime(time);
    });
}

void
loop() {
    envelope.noteOn();
    gigaDelay.noteOn();

    Serial.printf("A14: %d\n", analogRead(A14));
    Serial.printf("A15: %d\n", analogRead(A15));

    delay(1000);
}

} // namespace ssw

#endif
