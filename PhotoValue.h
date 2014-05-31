#ifndef __PHOTO_VALUE_H__

#define _BUFFER_LENGTH 10

#include <stdbool.h>
#include "Pins.h"

struct PhotoValue {
  enum Type {
    TRIGGER,
    TOGGLE,
    LINEAR    
  };
  
  Type type;
  float peak;
  float bottom;
  int lastTriggeredValue;
  unsigned long lastTriggeredTime;
  bool channelState;
  float hysteresis;
  int readbufferindex;
  long readbuffer[_BUFFER_LENGTH];
  long average;
  long deviation;
  int lastControlVal;

  PhotoValue();
  void calculate();
  void set(int v);
  bool isUp();
  bool isDown();
  void triggerUp(int controlNum, int i);
  void triggerDown(int controlNum, int i);

  bool isToggle() {
    return type == TOGGLE;
  }

  bool isTrigger() {
    return type == TRIGGER;
  }

  bool isLinear() {
    return type == LINEAR;
  }
};

static PhotoValue PHOTO_VALUES[_NUM_PHOTO_CONTROLS] = {PhotoValue(),PhotoValue(),PhotoValue(),PhotoValue()};

#endif __PHOTO_VALUE_H__
