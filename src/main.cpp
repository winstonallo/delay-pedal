#include "Config.hpp"

#ifdef SSW
#include "StereoSineWave.hpp"
#else
#include "PlayWavFile.hpp"
#endif

void
setup() {
    ssw::setup();
}

void
loop() {
    ssw::loop();
}
