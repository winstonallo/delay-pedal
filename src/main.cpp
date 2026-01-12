#include <Audio.h>

AudioSynthWaveformSine sound;
AudioControlSGTL5000 sgtl5000_1;
AudioOutputI2S i2s1;
AudioEffectEnvelope envelope;
AudioConnection connect1(sound, 0, envelope, 0);
AudioConnection connect2(sound, 0, envelope, 1);
AudioConnection connect3(envelope, 0, i2s1, 0);
AudioConnection connect4(envelope, 0, i2s1, 1);

void
setup() {
    AudioMemory(8);

    sound.amplitude(0.5);
    sound.frequency(440);
    sound.phase(180);

    envelope.attack(5);
    envelope.hold(50);

    envelope.decay(300);

    envelope.sustain(0);
    envelope.release(100);

    sgtl5000_1.enable();
    sgtl5000_1.volume(0.7);
}

void
loop() {
    envelope.noteOn();
    Serial.printf("envelope.isActive(): %d\n", envelope.isActive());
    delay(500);
}
