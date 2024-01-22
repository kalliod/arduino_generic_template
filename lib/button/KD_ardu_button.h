#pragma once

#include <Arduino.h>

class Button {

  private:
  int button_pin = 0;
  bool wasReleased = true;
  bool wasPressed = true;
  bool inverted = false;

  public:
  Button(int, bool, bool);
  Button(int, bool);
  Button(int);
  Button(void) { }
  void setPin(int);
  void setPullup(bool);
  void setInverted(bool);
  bool pressed(void);
  bool pressed(int);
  bool released(void);
  bool released(int);
  bool state(void);
  bool GetWasReleased(void);
  bool GetWasPressed(void);
};
