#include "Button.h"

Button BUTTONS[_NUM_BUTTON_CONTROLS] = {Button(BUTTONPINS[3]),Button(BUTTONPINS[2]),Button(BUTTONPINS[1]),Button(BUTTONPINS[0])};
 
void runButtonWorker() {
  for(int i = 0; i < _NUM_BUTTON_CONTROLS; ++i) {
    BUTTONS[i].read();
  }
}