#include "Config.hpp"

#ifdef SSW

#include "GigaDelay.hpp"
#include "StereoSineWave.hpp"
#include <Audio.h>

namespace ssw {
AudioControlSGTL5000 sgtl5000_1;
AudioOutputI2S i2s1;

AudioSynthWaveformSine sound1;
AudioSynthWaveformSine sound2;
GigaDelay envelope1;
GigaDelay envelope2;
AudioConnection connect1(sound1, 0, envelope1, 0);
AudioConnection connect2(sound1, 0, envelope1, 1);
AudioConnection connect3(sound2, 0, envelope2, 0);
AudioConnection connect4(sound2, 0, envelope2, 1);
AudioConnection connect5(envelope1, 0, i2s1, 0);
AudioConnection connect6(envelope2, 0, i2s1, 1);

Volume::Volume(uint8_t pin, uint32_t readInterval) : _pin(pin), _readInterval(readInterval), _curr(0), _sum(0), _filled(false) {
    memset(_samples, 0, VOLUME_SAMPLES * sizeof(uint16_t));
    _timer = IntervalTimer();
}

void
Volume::begin() {
    _timer.begin(ssw::updateVolume, _readInterval);
}

const uint8_t
Volume::getPin() const {
    return _pin;
}

void
Volume::addSample(uint16_t sample) {
    _sum -= _samples[_curr];

    _samples[_curr] = sample;
    _sum += sample;
    _curr++;

    if (_curr >= VOLUME_SAMPLES) {
        _curr = 0;
        _filled = true;
    }
}

const float32_t
Volume::getVolume() const {
    uint8_t count = _filled ? VOLUME_SAMPLES : (_curr == 0 ? VOLUME_SAMPLES : _curr);
    float32_t avgReading = (float32_t)_sum / (float32_t)count;
    float32_t vol = avgReading / 1023.0;

    return vol;
}

Volume volume(A0, 100000);

void
updateVolume() {
    uint16_t knobValue = analogRead(volume.getPin());
    volume.addSample(knobValue);
    float32_t vol = volume.getVolume();
    ssw::sgtl5000_1.volume(vol);
    Serial.printf("reading: %d, volume: %f\n", knobValue, vol);
}

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
    sgtl5000_1.volume(0.1);
    ssw::volume.begin();
}

void
loop() {
    envelope1.noteOn();
    envelope2.noteOn();

    delay(500);
}
} // namespace ssw

#endif
