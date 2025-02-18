#include "button.h"

// variables to manage the debouncing effect
int buttonTime = 0;
int lastButtonTime = 0;

// flag to manage the emergency
bool emergencyTriggered = false;

void managingEmergency()
{
    emergencyTriggered = false; // Reset flag
    writeToDisplay("Emergency!                ", "Rebooting...                ");
    rgb(255, 0, 0);
    closeServo();

    // disconnect all the clients
    refreshAP();

    // play a sad sound to notify the emergency
    playSadBuzzer();
    delay(5000);

    // restart the ESP so that we can go back to step 1
    esp_restart();
    return;
}
