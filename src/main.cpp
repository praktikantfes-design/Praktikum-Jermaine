#include <Arduino.h>
#include <iostream>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
using namespace std;

// LED1 an GPIO25, LED2 an GPIO26 angeschlossen
int led1 = 25;
int led2 = 26;
int buttonPin = 33;

int ScreenSda = 35;
int ScreenScl = 32;

// Zustände der LEDs (an/aus)

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define NUMFLAKES 10

int licht = analogRead(32);

void showWelcomePage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Wilkommen");
  display.display();
}


void Showlightpage() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print(map(analogRead(32), 0, 4095, 0, 100));
  display.println("%");
  display.setTextSize(1);
  display.println(analogRead(32));
  display.display();
}

void setup() {
  // Beide Pins als Ausgänge einstellen
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Programm Gestartet.");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println("Fehler beim initialisieren des Displays");
    for(;;);
  }
  display.clearDisplay();
  showWelcomePage();
}

int lastState = HIGH;

bool toggled = false;

int page = 0;


void loop() {
  // LICHTSENSOR

if (digitalRead(buttonPin) == LOW) {
    if (lastState == HIGH) {
      lastState = LOW;
      Serial.println("Button pressed");
      ++page;
      display.clearDisplay();
      display.display();
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      delay(100);
      digitalWrite(led1 ,LOW);
      digitalWrite(led2, LOW);
    }
    // Serial.println(lastState);
  } else {
    lastState = HIGH;
    // Serial.println(lastState);
  }
  if (page >= 2) page = 0;
  if (page == 0) {
    showWelcomePage();
  } else if (page == 1) {
    Showlightpage();
  }
}