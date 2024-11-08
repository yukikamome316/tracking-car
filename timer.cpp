#include "timer.h"

#include <Arduino.h>
#include <Servo.h>

bool Timer::Elapsed() {
  unsigned long current_millis = millis();
  if (current_millis - previous_millis_ >= interval_) {
    previous_millis_ = current_millis;
    return true;
  }
  return false;
}
