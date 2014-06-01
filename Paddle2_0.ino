//#define _DEBUG_MODE 1;
#define _MIDI_CONTROL_OFF 60

#ifndef _DEBUG_MODE
#include <Bounce.h>
#endif

#include "Pins.h"
#include "PhotoValue.h"
#include "Led.h"
#include "Button.h"

volatile bool BUTTON_VALUES[_NUM_BUTTON_CONTROLS] = {0,0,0,0};
int LED_COUNTS[_NUM_BUTTON_CONTROLS] = {0,0,0,0};
bool SETUP_MODE = false;

void setup() {
  Serial.begin(9600);
  Led::begin(200);
  Button::begin();

  for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
    pinMode(IRPINS[i], OUTPUT);
  }
}

void reactPhoto(int controlNum, int i) {
  digitalWrite(IRPINS[i], LOW);
  delay(1);
  int lowValue = analogRead(SENSORPINS[i]);
  digitalWrite(IRPINS[i], HIGH);
  delay(1);
  int highValue = analogRead(SENSORPINS[i]);

  int analogValue = lowValue - highValue;
  if(i == 2)
    Serial.println(analogValue);
  
  if(analogValue < 0)
    analogValue = 0;

  PhotoValue& pvalue = PHOTO_VALUES[i];
  pvalue.set(analogValue);

  if (pvalue.isUp()) {
    Serial.println("Up!");
    pvalue.triggerUp(controlNum, i);
  } else if (pvalue.isDown()) {
    Serial.println("Down!");
    pvalue.triggerDown(controlNum, i);
  }
}

void reactButton(int controlNum, int i) {
  if(BUTTONS[i].isPressed()) {
    if(BUTTON_VALUES[i] == 0) {
    //  usbMIDI.sendControlChange(controlNum, 127, 1);
   //   usbMIDI.sendControlChange(controlNum, 0, 1);
      BUTTON_VALUES[i] = 127;
    }
  } 
  
  if(BUTTONS[i].consumeTyped()){
    if(BUTTON_VALUES[i] != 0) {
      BUTTON_VALUES[i] = 0;
    }
  }
}

void checkSetupMode() {
  if(BUTTONS[0].isPressed() && BUTTONS[_NUM_BUTTON_CONTROLS - 1].isPressed()) {
    for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
      LEDS[i].on();
    }
    
    delay(200);
    
    for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
      LEDS[i].off();
    }
    
    SETUP_MODE = !SETUP_MODE;
    BUTTONS[0].consumePressed();
    BUTTONS[_NUM_BUTTON_CONTROLS - 1].consumePressed();
  }
}

void loop() { 
  ledTimer.run();
  buttonTimer.run();
  checkSetupMode();

  if(!(BUTTONS[0].isPressed() && BUTTONS[_NUM_BUTTON_CONTROLS - 1].isPressed())) {
    if(!SETUP_MODE) {
      for(int i = 0; i < _NUM_BUTTON_CONTROLS; ++i) {
	reactButton(_MIDI_CONTROL_OFF + _NUM_PHOTO_CONTROLS + i, i);  
      }
    
      for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
	reactPhoto(_MIDI_CONTROL_OFF + i, i);
      }
    } else {
      for(int i = 0; i < _NUM_BUTTON_CONTROLS; ++i) {
	if(BUTTONS[i].consumeTyped()) {
	  if(PHOTO_VALUES[i].isLinear())
	    PHOTO_VALUES[i].type = PhotoValue::TRIGGER;
	  else if(PHOTO_VALUES[i].isTrigger())
	    PHOTO_VALUES[i].type = PhotoValue::TOGGLE;
	  else if(PHOTO_VALUES[i].isToggle())
	    PHOTO_VALUES[i].type = PhotoValue::LINEAR;
	}
      }

      for(int i = 0; i < _NUM_PHOTO_CONTROLS; ++i) {
	if(PHOTO_VALUES[i].isToggle())
	  LEDS[i].on();
	else if(PHOTO_VALUES[i].isTrigger())
	  LEDS[i].off(); 
	else if(PHOTO_VALUES[i].isLinear())
	  LEDS[i].blink(); 
      }
    }
  }
}