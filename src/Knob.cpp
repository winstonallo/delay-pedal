#include "Knob.hpp"

Knob::Knob(uint8_t pin, uint32_t readInterval) : _pin(pin), _readInterval(readInterval), _curr(0), _sum(0), _filled(false) {
    memset(_samples, 0, KNOB_SAMPLES * sizeof(uint16_t));
    _timer = IntervalTimer();
}

void (*Knob::_updateCallback)() = nullptr;

void
Knob::begin(void (*updateCallback)()) {
    _updateCallback = updateCallback;

    if (_updateCallback) {
        _timer.begin(_updateCallback, _readInterval);
    }
}

uint8_t
Knob::getPin() const {
    return _pin;
}

float32_t
Knob::update() {
    uint16_t knobValue = _readFromPin();

    _sum -= _samples[_curr];

    _samples[_curr] = knobValue;
    _sum += knobValue;
    _curr++;

    if (_curr >= KNOB_SAMPLES) {
        _curr = 0;
        _filled = true;
    }

    return _getCurrentVolume();
}

float32_t
Knob::_getCurrentVolume() const {
    uint8_t count = _filled ? KNOB_SAMPLES : (_curr == 0 ? KNOB_SAMPLES : _curr);
    float32_t avgReading = (float32_t)_sum / (float32_t)count;
    float32_t vol = avgReading / 1023.0;

    return vol;
}

uint16_t
Knob::_readFromPin() {
    return analogRead(_pin);
}
