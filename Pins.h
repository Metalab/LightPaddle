#ifndef __PINS_H
#define __PINS_H

#define _NUM_PHOTO_CONTROLS 4
#define _NUM_BUTTON_CONTROLS 2

#include "Arduino.h"

static int BUTTONPINS[_NUM_BUTTON_CONTROLS] = {5,6};
static int IRPINS[_NUM_PHOTO_CONTROLS] = {14,15,16,17};
static int LEDPINS[_NUM_PHOTO_CONTROLS] = {0,1,2,3};
static int SENSORPINS[_NUM_PHOTO_CONTROLS] = {A0,A1,A2,A3};

#endif
