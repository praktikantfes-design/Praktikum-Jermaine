#include <Arduino.h>
#include <iostream>
using namespace std;

// LED1 an GPIO25, LED2 an GPIO26 angeschlossen
// LEDs an GPIO25 und GPIO26
int led1 = 25;
int led2 = 26;
int buttonPin = 33;

// Blinkgeschwindigkeiten (in Millisekunden)
int interval1 = 1000;   // LED1 wechselt alle 0,5 Sekunden
int interval2 = 300;   // LED2 wechselt alle 0,3 Sekunden

// Variablen, um zu speichern, wann zuletzt geschaltet wurde
unsigned long letzteZeit1 = 0;
unsigned long letzteZeit2 = 0;

// Zustände der LEDs (an/aus)
bool toggled = false;

void setup() {
  // Beide Pins als Ausgänge einstellen
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Programm Gestartet.");
}

void loop() {
  // // Erste LED an, zweite LED aus
  // digitalWrite(led1, HIGH);
  // digitalWrite(led2, LOW);
  // delay(500);  // kurze Pause

  // // Erste LED aus, zweite LED an
  // digitalWrite(led1, LOW);
  // digitalWrite(led2, HIGH);
  // delay(500);  // gleiche Pause
  // // aktuelle Zeit merken
  // unsigned long jetzt = millis();

  // --- LED1 prüfen ---
  // if (jetzt - letzteZeit1 >= interval1) {
  //   // Zustand umschalten
  //   led1An = !led1An;
  //   digitalWrite(led1, led1An ? HIGH : LOW);
  //   // Zeit merken
  //   letzteZeit1 = jetzt;
  // }

  // // --- LED2 prüfen ---
  // if (jetzt - letzteZeit2 >= interval2) {
  //   led2An = !led2An;
  //   digitalWrite(led2, led2An ? HIGH : LOW);
  //   letzteZeit2 = jetzt;
  // }

  int buttonState = digitalRead(buttonPin);
  
  // LICHT TOGGLE

  int lastState = HIGH;
  int currentState = HIGH;

      if (buttonState == LOW) {
        Serial.print("Current State: ");
        Serial.println(currentState);
        if (toggled != true) {
          digitalWrite(led1, HIGH);
          digitalWrite(led2, HIGH);
        }
        if (toggled == true) {
          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
        }
      }
      if (buttonState == HIGH) {
        if (toggled == false) {
          toggled = true;
        } else {
          toggled = false;
        }
      }
  // int clickcounter = 0;

  // if (buttonState == LOW) {
  //   ++clickcounter;
  //   digitalWrite(led1, HIGH);
  //   delay(1000);
  //   digitalWrite(led1, LOW);
  //   if (clickcounter == 2) {
  //     if (toggled == false) {
  //       digitalWrite(led2, HIGH);
  //       toggled = true;
  //     } else {
  //       digitalWrite(led2, LOW);
  //       toggled = false;
  //     }
  //     delay(200);
  //     clickcounter = 0;
  
  //   }
  // }

}