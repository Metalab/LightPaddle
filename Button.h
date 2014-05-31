#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "Pins.h"
#include "SimpleTimer.h"

void runButtonWorker();

static SimpleTimer buttonTimer;

struct Button {
private:
  int pin_;
  volatile bool pressed_ = false;
  volatile bool typed_ = false;
public:
  Button(int pin) : pin_(pin) {
  }

  static void begin() {
    for(int i = 0; i < _NUM_BUTTON_CONTROLS; ++i) {
      pinMode(BUTTONPINS[i], INPUT_PULLUP);
    } 
    
    buttonTimer.setInterval(10,runButtonWorker);
  }
  
  bool isPressed() volatile {
    return pressed_;
  }
  
  bool isTyped() volatile {
    return typed_;
  }

  bool consumeTyped() volatile{
    bool t = typed_;
    typed_ = false;
    return t;
  }
  
  void read() volatile {
    int val = digitalRead(BUTTONPINS[pin_]); 
    if(val == LOW) {
      pressed_ = true;
    } else if(pressed_) {
      typed_ = true;  
      pressed_ = false;
    } else {
      pressed_ = false;
    }
    //Serial.println(pressed_);
  }
};

extern Button BUTTONS[_NUM_BUTTON_CONTROLS];

#endif