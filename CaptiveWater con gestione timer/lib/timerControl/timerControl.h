#ifndef TIMERCONTROL_H
#define TIMERCONTROL_H

#include <Arduino.h>

#define TEMPO_25CL 10000
#define TEMPO_33CL 15000
#define TEMPO_50CL 20000
#define TEMPO_100CL 40000

extern int startTime;
extern int elapsedTime;
extern bool timerRunning;

void startTimer();
void stopTimer();
void restartTimer();
void resetTimer();
unsigned long int getElapsedTime();

#endif
