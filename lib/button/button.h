#ifndef BUTTON_H
#define BUTTON_H

#include "display.h"
#include "servo.h"
#include "rgb.h"
#include "buzzer.h"
#include "AP.h"

#define BUTTON 25

extern int buttonTime;
extern int lastButtonTime;
extern bool emergencyTriggered;

void managingEmergency();

#endif