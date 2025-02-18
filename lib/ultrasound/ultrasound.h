#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#define TRIG_PIN 5       // Pin trigger sensore ultrasuoni
#define ECHO_PIN 4         // Pin echo sensore ultrasuoni
#define MIN_DISTANCE 5.0 // Distanza minima in cm
#define MAX_DISTANCE 10.0 // Distanza massima in cm

float getDistance();



#endif
