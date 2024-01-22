#include "KD_ardu_button.h"

Button::Button(int _button_pin, bool _pullup, bool _inverted) {
  button_pin = _button_pin;
  setPullup(_pullup);
  setInverted(_inverted);
  wasReleased = !state();
}

Button::Button(int _button_pin, bool _pullup) {
  button_pin = _button_pin;
  setPullup(_pullup);
  wasReleased = !state();
}

Button::Button(int _button_pin) {
  button_pin = _button_pin;
  setPullup(false);
  wasReleased = !state();
}

void Button::setPullup(bool _pullup) {
  if(_pullup) {
    pinMode(button_pin, INPUT_PULLUP);
  } else {
    pinMode(button_pin, INPUT);
  }
}

// Returns HIGH if button is pressed down and LOW if the button is not being pressed
// Use this function if you want to keep doing something as long as the button is being pressed
bool Button::state() {
  if(!inverted && digitalRead(button_pin) || inverted && !digitalRead(button_pin)) { // button is pressed (inverted != digitalRead(button_pin))
    return true;  
  }
  else {
    return false;
    wasReleased = true;
  }
}

// Returns true if the button has been released and pressed
// Use this function if you only want to react to button press once
// You might need to do de-bounce in the code to avoid button bounce effect
bool Button::pressed() { 
  if(wasReleased && state()) {
    // Button has been released and pressed
    wasReleased = false;
    return true;
  } else if(state()) {
    // Button is continously pressed
    return false;
  } else if(!state()) {
    // Button is not pressed
    wasReleased = true;
    return false;
  }
}

// Same as previous, but with code blocking debounce
bool Button::pressed(int _DebounceDelay) { 
  if(wasReleased && state()) {
    // Button has been released and pressed
    delay(_DebounceDelay);
    if (state()) {
      wasReleased = false;
      wasPressed = true;
      return true;
    } else {
      return false;      
    }
  } else if(state()) {
    // Button is continously pressed
    return false;
  } else if(!state()) {
    // Button is not pressed
    wasReleased = true;
    return false;
  }
}

// Returns true if the button has been pressed and released
// Use this function if you only want to react to button press once
// You might need to do de-bounce in the code to avoid button bounce effect
bool Button::released() { 
  if(wasPressed && !state()) {
    // Button has been released and pressed
    wasReleased = true;
    wasPressed = false;
    return true;
  } else if(!state()) {
    // Button is not being pressed
    return false;
  } else if(state()) {
    // Button is pressed
    wasPressed = true;
    return false;
  }
}

// Same as previous, but with code blocking debounce
bool Button::released(int _DebounceDelay) { 
  if(wasPressed && !state()) {
    // Button has been released and pressed
    delay(_DebounceDelay);
    if (state()) {
      wasPressed = false;
      wasReleased = true;
      return true;
    } else {
      return false;      
    }
  } else if(!state()) {
    // Button is not being pressed
    return false;
  } else if(state()) {
    // Button is pressed
    wasPressed = true;
    return false;
  }
}

// Returns true if the button has been released after previous pressing down
bool Button::GetWasReleased() {
  return wasReleased;
}

// Returns true if the button has been pressed down after previous release
bool Button::GetWasPressed() {
  return wasPressed;
}

void Button::setPin(int _button_pin) 
{ 
  button_pin = _button_pin; 
}

void Button::setInverted(bool _invert) 
{
  inverted = _invert; 
  wasReleased = !state();
}
