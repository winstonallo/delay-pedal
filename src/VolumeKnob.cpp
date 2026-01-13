#include "VolumeKnob.hpp"

VolumeKnob::VolumeKnob(uint8_t pin, uint32_t readInterval) : _pin(pin), _readInterval(readInterval), _curr(0), _sum(0), _filled(false) {
    memset(_samples, 0, VOLUME_SAMPLES * sizeof(uint16_t));
    _timer = IntervalTimer();
}

void (*VolumeKnob::_updateCallback)() = nullptr;

void
VolumeKnob::begin(void (*updateCallback)()) {
    _updateCallback = updateCallback;

    if (_updateCallback) {
        _timer.begin(_updateCallback, _readInterval);
    }
}

uint8_t
VolumeKnob::getPin() const {
    return _pin;
}

float32_t
VolumeKnob::update() {
    uint16_t knobValue = _readFromPin();

    _sum -= _samples[_curr];

    _samples[_curr] = knobValue;
    _sum += knobValue;
    _curr++;

    if (_curr >= VOLUME_SAMPLES) {
        _curr = 0;
        _filled = true;
    }

    return _getCurrentVolume();
}

float32_t
VolumeKnob::_getCurrentVolume() const {
    uint8_t count = _filled ? VOLUME_SAMPLES : (_curr == 0 ? VOLUME_SAMPLES : _curr);
    float32_t avgReading = (float32_t)_sum / (float32_t)count;
    float32_t vol = avgReading / 1023.0;

    return vol;
}

uint16_t
VolumeKnob::_readFromPin() {
    return analogRead(_pin);
}
