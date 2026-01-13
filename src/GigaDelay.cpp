#include "GigaDelay.hpp"
#include "AudioStream.h"
#include <Arduino.h>
#include <cstddef>
#include <cstdint>
#include <cstring>

#define STATE_IDLE 0
#define STATE_DELAY 1
#define STATE_ATTACK 2
#define STATE_HOLD 3
#define STATE_DECAY 4
#define STATE_SUSTAIN 5
#define STATE_RELEASE 6
#define STATE_FORCED 7

void
GigaDelay::noteOn(void) {
    __disable_irq();
    if (state == STATE_IDLE || state == STATE_DELAY || release_forced_count == 0) {
        mult_hires = 0;
        count = delay_count;
        if (count > 0) {
            state = STATE_DELAY;
            inc_hires = 0;
        } else {
            state = STATE_ATTACK;
            count = attack_count;
            inc_hires = 0x40000000 / (int32_t)count;
        }
    } else if (state != STATE_FORCED) {
        state = STATE_FORCED;
        count = release_forced_count;
        inc_hires = (-mult_hires) / (int32_t)count;
    }
    __enable_irq();
}

void
GigaDelay::noteOff(void) {
    __disable_irq();
    if (state != STATE_IDLE && state != STATE_FORCED) {
        state = STATE_RELEASE;
        count = release_count;
        inc_hires = (-mult_hires) / (int32_t)count;
    }
    __enable_irq();
}

void
GigaDelay::setTime(size_t time_ms) {
    _time_ms = time_ms;
}

#define GIGADELAY_TIME_SEC 1
#define GIGADELAY_RINGBUFFER_SIZE ((int)AUDIO_SAMPLE_RATE)

class RingBuffer {
  private:
    uint16_t _buf[GIGADELAY_RINGBUFFER_SIZE];
    size_t _curr;
    size_t _delay_samples;

  public:
    RingBuffer(size_t time_ms) {
        _curr = 0;
        _delay_samples = (time_ms * AUDIO_SAMPLE_RATE) / 1000;
        if (_delay_samples >= GIGADELAY_RINGBUFFER_SIZE) {
            _delay_samples = GIGADELAY_RINGBUFFER_SIZE - 1;
        }
        memset(_buf, 0, sizeof(_buf));
    }

    void
    addSample(uint16_t sample) {
        _buf[_curr] = sample;
        _curr = (_curr + 1) % GIGADELAY_RINGBUFFER_SIZE;
    }

    void
    update(audio_block_t *block) {
        for (size_t idx = 0; idx < AUDIO_BLOCK_SAMPLES; ++idx) {
            int16_t delayed_sample = getDelayedSample();
            int32_t mixed = (block->data[idx] * 7 + delayed_sample * 3) / 10;
            block->data[idx] = (int16_t)mixed;
            addSample(block->data[idx]);
        }
    }

    uint16_t
    getDelayedSample() {
        size_t read_pos = (_curr + GIGADELAY_RINGBUFFER_SIZE - _delay_samples) % GIGADELAY_RINGBUFFER_SIZE;
        return _buf[read_pos];
    }

    void
    setTime(size_t time_ms) {
        _delay_samples = (time_ms * AUDIO_SAMPLE_RATE) / 1000;
        if (_delay_samples >= GIGADELAY_RINGBUFFER_SIZE) {
            _delay_samples = GIGADELAY_RINGBUFFER_SIZE - 1;
        }
    }
};

RingBuffer buffer(1000);

void
GigaDelay::update(void) {
    static int round = 0;
    static int coef = 1;

    if (round % 100 == 0) {
        coef = 2; // rand() % 10;
    }

    audio_block_t *block = receiveWritable();
    if (!block) return;
    if (state == STATE_IDLE) {
        AudioStream::release(block);
        return;
    }

    buffer.setTime(_time_ms);

    buffer.update(block);

    transmit(block);
    AudioStream::release(block);
    round++;
}

bool
GigaDelay::isActive() {
    uint8_t current_state = *(volatile uint8_t *)&state;
    if (current_state == STATE_IDLE) return false;
    return true;
}

bool
GigaDelay::isSustain() {
    uint8_t current_state = *(volatile uint8_t *)&state;
    if (current_state == STATE_SUSTAIN) return true;
    return false;
}
