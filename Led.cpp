#include "Led.h"

Led LEDS[_NUM_PHOTO_CONTROLS] = {Led(LEDPINS[0]), Led(LEDPINS[1]), Led(LEDPINS[2]), Led(LEDPINS[3])};

void runLedWorkers() { 
  for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
    if(LEDS[i].isBlinking()) 
      LEDS[i].toggle();
    }
}