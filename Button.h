#ifndef __BUTTONS_H
#define __BUTTONS_H

#include "Pins.h"
#include "SimpleTimer.h"

void runButtonWorker();

static SimpleTimer buttonTimer;

struct Button {
private:
  int pin_;
  volatile bool pressed_;
  volatile bool typed_;
  long lastPress_;
public:
  Button(int pin) : pin_(pin), pressed_(false), typed_(false), lastPress_(millis()) {
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

  bool consumeTyped() volatile {
    if(typed_) {
      typed_ = false;
      return true;
    }
    return false;
  }
  
  bool consumePressed() volatile {
    if(pressed_) {
      pressed_ = false;
      return true;
    }
    return false;
  }
  
  void read() volatile {
    int val = digitalRead(BUTTONPINS[pin_]); 
    long now = millis();
    
    if(val == LOW) {
      if(!pressed_ || lastPress_ + 1000 < now) {
	lastPress_ = now;
	pressed_ = true;
      }
    } else if(pressed_) {
      typed_ = true;  
      pressed_ = false;
    }
    //Serial.println(pressed_);
  }
};

extern Button BUTTONS[_NUM_BUTTON_CONTROLS];

#endif
