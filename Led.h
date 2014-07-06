#ifndef __LEDS_H
#define __LEDS_H

#include "Arduino.h"
#include "Pins.h"
#include "SimpleTimer.h"

void runLedWorkers();

static SimpleTimer ledTimer;

class Led {
private:
  int pin_;
  volatile bool blink_;
  volatile bool state_;
public:
  Led(int pin) : pin_(pin), blink_(false), state_(true) {
  }
  
  static void begin(int timeout) {  
    for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
      pinMode(LEDPINS[i], OUTPUT);
    }
    
    ledTimer.setInterval(timeout, runLedWorkers);
  }
  
  void on() volatile {
    blink_ = false;
    state_ = true;
    digitalWrite(pin_, HIGH);
  }
  
  void off() volatile {
    blink_ = false;
    state_ = false;
    digitalWrite(pin_, LOW);
  }
  
  void blink() volatile {
    blink_ = true;
  }
  
  bool isBlinking() volatile {
    return blink_;
  }
  
  void toggle() volatile {
    if(state_) {
      digitalWrite(pin_, LOW);
      state_ = false;
    } else {
      digitalWrite(pin_, HIGH);
      state_ = true;
    }
  }
};

extern Led LEDS[_NUM_PHOTO_CONTROLS];

#endif
