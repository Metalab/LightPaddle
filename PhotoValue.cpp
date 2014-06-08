#include "PhotoValue.h"
#include "Pins.h"
#include "Arduino.h"

PhotoValue::PhotoValue() : type(TRIGGER), peak(1023.0), bottom(0.0), 
  lastTriggeredValue(0), lastTriggeredTime(0), 
  channelState(false), hysteresis(100.0),
  readbufferindex(0), average(0), deviation(0), lastControlVal(0) {
  
  for (int i = 0; i < _BUFFER_LENGTH; i++) {
      readbuffer[i] = 0;
  }
}

void PhotoValue::calculate() {
  long sum = 0;
  long last = readbuffer[0];
  long diff = 0;
  long totalDiff = 0;
  
  for (int i = 0; i < _BUFFER_LENGTH; i++) {
    diff = last - readbuffer[i];
    sum += (last = readbuffer[i]);
    totalDiff += diff;
  }

  deviation = totalDiff / _BUFFER_LENGTH;
  average =  sum / _BUFFER_LENGTH;
  
  if (bottom + 15 <= peak) {
    peak -= 5;
    bottom += 5;
  }
  
  if (average > peak) {
    peak += 5.0;
  }
  if (average < bottom) {
    bottom -= 5.0;
  }
  float centre = (bottom + peak) / 2.0;
  hysteresis = abs(peak - bottom) / 2.2;
}

void PhotoValue::set(int v) {
  readbuffer[readbufferindex] = v;
  ++readbufferindex;
  if (readbufferindex == _BUFFER_LENGTH) {
    readbufferindex = 0;
  }

  calculate();
}

bool PhotoValue::isUp() {
  /*Serial.print(average);
   Serial.print(" : ");
   Serial.print(peak);
   Serial.print(" : ");
   Serial.println(hysteresis);*/
  return (isLinear() && average > 5) || (!isLinear() && !channelState && (average > (peak - hysteresis)) && (lastTriggeredTime + 350 <= millis()));
}

bool PhotoValue::isDown() {
  return (isLinear() && !isUp()) || (channelState  && average < (bottom + hysteresis));
}


void PhotoValue::triggerUp(int controlNum, int i) {
  lastTriggeredValue = average;
  channelState = true;
  
  lastTriggeredTime = millis();
  
  int controlVal = 127;
  if(isToggle()) {
    controlVal = lastControlVal ? 0 : 127;
  } else if(isLinear()) {
    controlVal = average;
    if(controlVal > 127)
      controlVal = 127;
  }
  
#ifndef _DEBUG_MODE
  if(!isLinear() || (controlVal != 0))
    usbMIDI.sendControlChange(controlNum,controlVal, 1);
#endif
  
  if(isToggle()) {
    if(controlVal)
      digitalWrite(LEDPINS[i], HIGH);
    else
      digitalWrite(LEDPINS[i], LOW);
  } else
    digitalWrite(LEDPINS[i], HIGH);

  
  lastControlVal = controlVal;
}

void PhotoValue::triggerDown(int controlNum, int i) {
  lastTriggeredValue = average;
  channelState = false;
  
  if(!isToggle()) {
    digitalWrite(LEDPINS[i], LOW);
  }
  if(isTrigger()) {
#ifndef _DEBUG_MODE
    usbMIDI.sendControlChange(controlNum,0, 1);
#endif
  }
}
