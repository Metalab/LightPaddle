#ifndef __PINS_H
#define __PINS_H

#define _NUM_PHOTO_CONTROLS 4
#define _NUM_BUTTON_CONTROLS 4

#include "Arduino.h"

static int BUTTONPINS[_NUM_BUTTON_CONTROLS] = {0,1,2,3};
static int IRPINS[_NUM_PHOTO_CONTROLS] = {8,9,10,11};
static int LEDPINS[_NUM_PHOTO_CONTROLS] = {4,5,6,7};
static int SENSORPINS[_NUM_PHOTO_CONTROLS] = {A9,A8,A7,A6};

#endif