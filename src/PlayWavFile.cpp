#include "Config.hpp"

#ifdef WVK

#include "PlayWavFile.hpp"
#include "VolumeKnob.hpp"
#include <Audio.h>

namespace pwf {

AudioPlaySdWav playSdWav1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

VolumeKnob volume(A14, 100000);

void
setup() {
    Serial.begin(9600);
    AudioMemory(8);

    pwf::sgtl5000_1.enable();
    pwf::sgtl5000_1.volume(0.1);

    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);

    while (!(SD.begin(SDCARD_CS_PIN))) {
        Serial.println("Unable to access the SD card");
        delay(500);
    }

    pinMode(LED_BUILTIN, OUTPUT);

    pwf::volume.begin([] {
        float32_t vol = volume.update();
        pwf::sgtl5000_1.volume(vol);
    });

    delay(1000);
}

void
loop() {
    if (pwf::playSdWav1.isPlaying() == false) {
        Serial.println("Start playing");
        pwf::playSdWav1.play("A.WAV");
        delay(10);
    }

    Serial.printf("A14: %d\n", analogRead(A14));

    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
}

} // namespace pwf

#endif
