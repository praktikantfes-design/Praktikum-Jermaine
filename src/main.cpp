#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

int led1 = 25;
int led2 = 26;

int interval1 = 500;
int interval2 = 300;

unsigned long letzteZeit1 = 0;
unsigned long letzteZeit2 = 0;

bool led1An = false;
bool led2An = false;

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(26, OUTPUT);
  pinMode(25, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  delay(500);

  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  delay(500);

  unsigned long jetzt = millis();

  if (jetzt - letzteZeit1 >=interval1) {
    led1An = !led1An;
    digitalWrite(led1, led1An ? HIGH : LOW);
    letzteZeit1 = jetzt;
  }


}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}