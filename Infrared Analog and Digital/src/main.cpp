#include <Arduino.h>

#define DIGITAL_PIN 13
#define ANALOG_PIN 34
#define BTN 12
#define LED 14
int lastState = HIGH;
int currentState;
int ledReady;

//prova jago

//function declarations here:
int isLedReady();
int buttonPressed();
void analogValue();

void setup() {
  Serial.begin(9600);
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(ANALOG_PIN, INPUT);
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  
}

void loop() {
  analogValue(); //stampo il valore dell'ingresso analogico (sensore infrarossi)

  //controllo se il pulsante è stato premuto
  if(buttonPressed() == LOW){  //se il pulsante non è premuto, il led si spegne
      digitalWrite(LED, LOW);
    }
  if(buttonPressed() == HIGH && isLedReady() == HIGH){ //se il pulsante è premuto e c'è qualcosa davanti al sensore, il led si accende
    digitalWrite(LED, HIGH);
  }
  if(buttonPressed() == HIGH && isLedReady() == LOW){ //se il pulsante è premuto ma non c'è nulla davanti al sensore, il led si spegne
    digitalWrite(LED, LOW);
  }
}

// put function definitions here:
int isLedReady(){
  int digitalValue = digitalRead(DIGITAL_PIN); //leggo il valore dell'ingresso digitale (sensore infrarossi)
  if(digitalValue == LOW){ //se il valore è basso, c'è qualcosa davanti al sensore
    return HIGH;
  }
  else{ //se il valore è alto, non c'è nulla davanti al sensore
    return LOW;
  }
}

int buttonPressed(){
  //il pulsante va gestito con un resistore di pull-up. Devo quindi leggere il valore del pulsante e fare il confronto con l'ultimo stato
  int currentState = digitalRead(BTN);
  if(lastState == HIGH && currentState == LOW){
    //Serial.println("schiacciato");
    return HIGH;
  }
  else{
    //Serial.println("no schiacciato");
    return LOW;
  }
  lastState = currentState;
}

void analogValue(){ //funzione per leggere il valore dell'ingresso analogico (sensore infrarossi)
  int value = analogRead(ANALOG_PIN);
  Serial.println(value);
}
