#include "button.h"


int buttonTime = 0;  
int lastButtonTime = 0; 
bool emergencyTriggered = false;

void managingEmergency(){
    emergencyTriggered = false; // Reset flag
    scriviDisplay("Emergenza!                ", "Riavvio in corso...                ");
    rgb(255, 0, 0);
    closeServo();
  
    //stacco i client connessi
    refreshAP();
  
    // FAR SUONARE BUZZER PER AVVISARE
    playSadBuzzer();
    delay(5000);
  
    // Riavvio ESP
    esp_restart();
    return;
}

//   void IRAM_ATTR isr() {
//     buttonTime = millis();
//     //we're careful about the deboucing effect that can occur
//     if (buttonTime - lastButtonTime > 250)
//     {
//       emergencyTriggered = true;
//       lastButtonTime = buttonTime;
//     }
//   }
