#include "Config.hpp"

#ifdef SSW

#include <Audio.h>

namespace ssw {
AudioControlSGTL5000 sgtl5000_1;
AudioOutputI2S i2s1;

AudioSynthWaveformSine sound1;
AudioSynthWaveformSine sound2;
AudioEffectEnvelope envelope1;
AudioEffectEnvelope envelope2;
AudioConnection connect1(sound1, 0, envelope1, 0);
AudioConnection connect2(sound1, 0, envelope1, 1);
AudioConnection connect3(sound2, 0, envelope2, 0);
AudioConnection connect4(sound2, 0, envelope2, 1);
AudioConnection connect5(envelope1, 0, i2s1, 0);
AudioConnection connect6(envelope2, 0, i2s1, 1);

void
setup() {
    AudioMemory(8);

    sound1.amplitude(0.5);
    sound1.frequency(300);
    sound1.phase(180);

    sound2.amplitude(0.5);
    sound2.frequency(220);
    sound2.phase(180);

    envelope1.attack(5);
    envelope1.hold(50);

    envelope1.decay(300);

    envelope1.sustain(0);
    envelope1.release(100);

    envelope2.attack(5);
    envelope2.hold(50);

    envelope2.decay(300);

    envelope2.sustain(0);
    envelope2.release(100);

    sgtl5000_1.enable();
    sgtl5000_1.volume(0.3);
}

void
loop() {
    envelope1.noteOn();
    envelope2.noteOn();

    delay(500);
}
} // namespace ssw

#endif