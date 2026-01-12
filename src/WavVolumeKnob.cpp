#include "Config.hpp"

#ifdef WVK

#include "WavVolumeKnob.hpp"
#include <Audio.h>

namespace wvk {
AudioPlaySdWav playSdWav1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

Volume::Volume(uint8_t pin, uint32_t readInterval) : _pin(pin), _readInterval(readInterval), _curr(0), _sum(0), _filled(false) {
    memset(_samples, 0, VOLUME_SAMPLES * sizeof(uint16_t));
    _timer = IntervalTimer();
}

void
Volume::begin() {
    _timer.begin(wvk::updateVolume, _readInterval);
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
    wvk::sgtl5000_1.volume(vol);
    Serial.printf("reading: %d, volume: %f\n", knobValue, vol);
}

void
setup() {
    Serial.begin(9600);
    AudioMemory(8);
    wvk::sgtl5000_1.enable();
    wvk::sgtl5000_1.volume(0.1);
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    while (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        delay(500);
    }
    pinMode(LED_BUILTIN, OUTPUT);
    wvk::volume.begin();
    delay(1000);
}

void
loop() {
    if (wvk::playSdWav1.isPlaying() == false) {
        Serial.println("Start playing");
        wvk::playSdWav1.play("A.WAV");
        delay(10);
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

    delay(100);
}
} // namespace wvk

#endif