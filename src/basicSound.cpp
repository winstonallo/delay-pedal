#include <Audio.h>

AudioSynthWaveformSine	sound;
AudioControlSGTL5000 sgtl5000_1;
AudioOutputI2S i2s1;
AudioConnection connect(sound, 0, i2s1, 0);
AudioEffectEnvelope envelope;


void setup() {
	AudioMemory(8);
	sound.amplitude(0.1);
	sound.frequency(440);
	sound.phase(180);
	envelope.attack(10);
	envelope.hold(100);
	envelope.sustain(0.5f);
	envelope.release(20);
	sgtl5000_1.enable();
    sgtl5000_1.volume(0.1);
}

void loop() {
	envelope.noteOn();
	delay(5000);
}