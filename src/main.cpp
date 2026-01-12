// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 1-3: First "Hello World" program, play a music file
//
// WAV files for this and other Tutorials are here:
// http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html

#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/io.h>

AudioPlaySdWav playSdWav1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

// Use these with the Teensy Audio Shield
// #define SDCARD_CS_PIN 10
// #define SDCARD_MOSI_PIN 7 // Teensy 4 ignores this, uses pin 11
// #define SDCARD_SCK_PIN 14 // Teensy 4 ignores this, uses pin 13

// Use these with the Teensy 3.5 & 3.6 &
// 4.1 SD card
#define SDCARD_CS_PIN BUILTIN_SDCARD
#define SDCARD_MOSI_PIN 11 // not actually used
#define SDCARD_SCK_PIN 13  // not actually used

int volumeKnob = A0;
int volumeValues = 0;

// Use these for the SD+Wiz820 or other adaptors
// #define SDCARD_CS_PIN    4
// #define SDCARD_MOSI_PIN  11l
// #define SDCARD_SCK_PIN   13

void updateVolume();

#define VOLUME_SAMPLES 5

class Volume {
  private:
    uint8_t _pin;
    uint32_t _readInterval;
    uint16_t _samples[VOLUME_SAMPLES];
    uint8_t _curr;
    uint16_t _sum;
    bool _filled;
    IntervalTimer _timer;

  public:
    Volume(uint8_t pin, uint32_t readInterval) : _pin(pin), _readInterval(readInterval), _curr(0), _sum(0), _filled(false) {
        memset(_samples, 0, VOLUME_SAMPLES * sizeof(uint16_t));
        _timer = IntervalTimer();
    }

    Volume() = delete;

    void
    begin() {
        _timer.begin(updateVolume, _readInterval);
    }

    const uint8_t
    getPin() const {
        return _pin;
    }

    void
    addSample(uint16_t sample) {
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
    getVolume() const {
        uint8_t count = _filled ? VOLUME_SAMPLES : (_curr == 0 ? VOLUME_SAMPLES : _curr);
        float32_t avgReading = (float32_t)_sum / (float32_t)count;
        float32_t vol = avgReading / 1023.0;

        return vol;
    }
};

Volume volume(A0, 100000);

void
updateVolume() {
    uint16_t knobValue = analogRead(volume.getPin());
    volume.addSample(knobValue);
    float32_t vol = volume.getVolume();
    sgtl5000_1.volume(vol);
    Serial.printf("reading: %d, volume: %f\n", knobValue, vol);
}

volatile uint32_t sampleCount = 0;

void
setup() {
    Serial.begin(9600);
    AudioMemory(8);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.1);
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    while (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        delay(500);
    }
    pinMode(LED_BUILTIN, OUTPUT);
    volume.begin();
    delay(1000);
}

void
loop() {
    if (playSdWav1.isPlaying() == false) {
        Serial.println("Start playing");
        playSdWav1.play("A.WAV");
        delay(10);
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

    delay(100);
}
