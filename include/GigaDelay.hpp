#ifndef GIGADELAY_HPP
#define GIGADELAY_HPP

#include "utility/dspinst.h"
#include <Arduino.h>     // github.com/PaulStoffregen/cores/blob/master/teensy4/Arduino.h
#include <AudioStream.h> // github.com/PaulStoffregen/cores/blob/master/teensy4/AudioStream.h

#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT / 1000.0f)

class GigaDelay : public AudioStream {

  public:
    GigaDelay() : AudioStream(1, inputQueueArray) {
        state = 0;
        delay(0.0f); // default values...
        attack(10.5f);
        hold(2.5f);
        decay(35.0f);
        sustain(0.5f);
        release(300.0f);
        releaseNoteOn(5.0f);
    }
    void noteOn();
    void noteOff();
    void
    delay(float milliseconds) {
        delay_count = milliseconds2count(milliseconds);
    }
    void
    attack(float milliseconds) {
        attack_count = milliseconds2count(milliseconds);
        if (attack_count == 0) attack_count = 1;
    }
    void
    hold(float milliseconds) {
        hold_count = milliseconds2count(milliseconds);
    }
    void
    decay(float milliseconds) {
        decay_count = milliseconds2count(milliseconds);
        if (decay_count == 0) decay_count = 1;
    }
    void
    sustain(float level) {
        if (level < 0.0f)
            level = 0;
        else if (level > 1.0f)
            level = 1.0f;
        sustain_mult = level * 1073741824.0f;
    }
    void
    release(float milliseconds) {
        release_count = milliseconds2count(milliseconds);
        if (release_count == 0) release_count = 1;
    }
    void
    releaseNoteOn(float milliseconds) {
        release_forced_count = milliseconds2count(milliseconds);
        if (release_count == 0) release_count = 1;
    }
    bool isActive();
    bool isSustain();
    virtual void update(void);

  private:
    uint16_t
    milliseconds2count(float milliseconds) {
        if (milliseconds < 0.0f) milliseconds = 0.0f;
        uint32_t c = ((uint32_t)(milliseconds * SAMPLES_PER_MSEC) + 7) >> 3;
        if (c > 65535) c = 65535; // allow up to 11.88 seconds
        return c;
    }
    audio_block_t *inputQueueArray[1];
    // state
    uint8_t state;      // idle, delay, attack, hold, decay, sustain, release, forced
    uint16_t count;     // how much time remains in this state, in 8 sample units
    int32_t mult_hires; // attenuation, 0=off, 0x40000000=unity gain
    int32_t inc_hires;  // amount to change mult_hires every 8 samples

    // settings
    uint16_t delay_count;
    uint16_t attack_count;
    uint16_t hold_count;
    uint16_t decay_count;
    int32_t sustain_mult;
    uint16_t release_count;
    uint16_t release_forced_count;
};

#endif
