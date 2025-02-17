#include "buzzer.h"
#include "pitches.h"

const int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
const int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void suonaBuzzerFelice(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER_PIN, melody[thisNote], noteDuration);
  
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(BUZZER_PIN);
    }
  
  }
  void suonaBuzzerTriste() {
    tone(BUZZER_PIN, 880, 500);  // La5
    delay(600);
    tone(BUZZER_PIN, 698, 500);  // Fa5
    delay(600);
    tone(BUZZER_PIN, 523, 500);  // Do5
    delay(600);
    noTone(BUZZER_PIN);          // Spegne il buzzer
  }