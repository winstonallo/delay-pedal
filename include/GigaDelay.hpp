#ifndef GIGADELAY_HPP
#define GIGADELAY_HPP

#include <Arduino.h>
#include <AudioStream.h>
#include <ctime>

#ifndef GIGADELAY_SAMPLES_PER_MSEC
#define GIGADELAY_SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT / 1000.0f)
#endif

class GigaDelay : public AudioStream {

  public:
    GigaDelay() : AudioStream(1, inputQueueArray), _ms(1000) { state = 0; }
    void noteOn();
    void noteOff();

    void setTime(size_t ms);

    bool isActive();
    bool isSustain();
    virtual void update(void);

  private:
    uint16_t
    milliseconds2count(float milliseconds) {
        if (milliseconds < 0.0f) milliseconds = 0.0f;
        uint32_t c = ((uint32_t)(milliseconds * GIGADELAY_SAMPLES_PER_MSEC) + 7) >> 3;
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
    size_t _ms;
};

#endif
