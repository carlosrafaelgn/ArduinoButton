//
// ArduinoButton is distributed under the FreeBSD License
//
// Copyright (c) 2015, Carlos Rafael Gimenes das Neves
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies,
// either expressed or implied, of the FreeBSD Project.
//
// https://github.com/carlosrafaelgn/ArduinoButton
//

#ifndef ArduinoButton_h
#define ArduinoButton_h

#include <Arduino.h>
#include <inttypes.h>

// If your buttons are active-low (their pins become 0 when pressed,
// and 1 when released), please define ArduinoButtonPressedZero before
// including this library
#ifdef ArduinoButtonPressedZero
  #define ArduinoButtonPressed 0
  #define ArduinoButtonReleased 1
#else
  #define ArduinoButtonPressed 1
  #define ArduinoButtonReleased 0
#endif

// Allow delay customization
#ifndef ArduinoButtonDebounceDelay
  #define RemoveArduinoButtonDebounceDelay
  #define ArduinoButtonDebounceDelay 40
#endif

// Allow integration to other IO libraries
#ifndef ArduinoButtonRead
  #define RemoveArduinoButtonRead
  #define ArduinoButtonRead digitalRead
#endif

#ifndef ArduinoButtonMode
  #define RemoveArduinoButtonMode
  #define ArduinoButtonMode pinMode
#endif

#define ArduinoButtonEnabled 2

typedef void (* ARDUINOBUTTONPROC)();

class Button {
private:
  uint8_t pin, state;
  uint32_t lastTime;
  ARDUINOBUTTONPROC procPress, procRelease;

public:
  void begin(uint8_t pin, ARDUINOBUTTONPROC procPress, ARDUINOBUTTONPROC procRelease = 0) {
    ArduinoButtonMode(pin, INPUT);
    this->pin = pin;
    state = ArduinoButtonEnabled | ArduinoButtonRead(pin);
    lastTime = 0;
    this->procPress = procPress;
    this->procRelease = procRelease;
  }

  inline bool isPressed() {
#ifdef ArduinoButtonPressedZero
    return ((state & 1) ^ 1);
#else
    return (state & 1);
#endif
  }

  inline bool isEnabled() {
    return ((state >> 1) & 1);
  }

  inline void enable() {
    state |= ArduinoButtonEnabled;
  }

  inline void disable() {
    state &= (~ArduinoButtonEnabled);
  }

  void process() {
    if ((state & ArduinoButtonEnabled)) {
      uint8_t s = (ArduinoButtonEnabled | ArduinoButtonRead(pin));
      if (s != state) {
#ifndef ArduinoButtonGlobalClock
        uint32_t now = millis();
        if ((now - lastTime) > ArduinoButtonDebounceDelay) {
          lastTime = now;
#else
        if ((ArduinoButtonGlobalClock - lastTime) > ArduinoButtonDebounceDelay) {
          lastTime = ArduinoButtonGlobalClock;
#endif
          state = s;
          if (s != (ArduinoButtonEnabled | ArduinoButtonReleased)) {
            if (procPress)
              procPress();
          } else {
            if (procRelease)
              procRelease();
          }
        } else {
          // Debounce!
#ifndef ArduinoButtonGlobalClock
          lastTime = now;
#else
          lastTime = ArduinoButtonGlobalClock;
#endif
        }
      }
    }
  }

  // Simulate a key press
  void press() {
    if (state == (ArduinoButtonEnabled | ArduinoButtonReleased)) {
      state = (ArduinoButtonEnabled | ArduinoButtonPressed);
      if (procPress)
        procPress();
    }
  }

  // Simulate a key press
  void release() {
    if (state == (ArduinoButtonEnabled | ArduinoButtonPressed)) {
      state = (ArduinoButtonEnabled | ArduinoButtonReleased);
      if (procRelease)
        procRelease();
    }
  }
};

#undef ArduinoButtonPressed
#undef ArduinoButtonReleased

#ifdef RemoveArduinoButtonDebounceDelay
  #undef RemoveArduinoButtonDebounceDelay
  #undef ArduinoButtonDebounceDelay
#endif

#ifdef RemoveArduinoButtonRead
  #undef RemoveArduinoButtonRead
  #undef ArduinoButtonRead
#endif

#ifdef RemoveArduinoButtonMode
  #undef RemoveArduinoButtonMode
  #undef ArduinoButtonMode
#endif

#undef ArduinoButtonEnabled

#endif
