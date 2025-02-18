#include "buzzer.h"
#include "pitches.h"

// Melody for the buzzer. The notes are defined in pitches.h
const int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
const int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

void playHappyBuzzer()
{
  for (int thisNote = 0; thisNote < 8; thisNote++)
  {
    int noteDuration = 1000 / noteDurations[thisNote]; // calculate the duration of the notes
    tone(BUZZER_PIN, melody[thisNote], noteDuration);  // play the note

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);
  }
}
void playSadBuzzer()
{
  tone(BUZZER_PIN, 880, 500);
  delay(600);
  tone(BUZZER_PIN, 698, 500);
  delay(600);
  tone(BUZZER_PIN, 523, 500);
  delay(600);
  noTone(BUZZER_PIN); // stop the tone playing
}