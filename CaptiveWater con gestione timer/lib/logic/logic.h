#ifndef LOGIC_H
#define LOGIC_H

#include "timerControl.h"
#include "display.h"
#include "rgb.h"
#include "servo.h"
#include "AP.h"
#include "buzzer.h"
#include <Arduino.h>

extern bool isPouring;

void terminateWaterRelease();
void activateWaterRelease();
void stopWaterRelease();

#endif
