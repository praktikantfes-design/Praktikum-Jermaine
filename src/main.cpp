#include <Arduino.h>
#include <iostream>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <wifi_credentials.h>
#include <WebServer.h>
using namespace std;

WebServer server(80);

/*
      Variablen
                    */ 

int lastState = HIGH;
int page = 0;
bool toggled = false;

int led1 = 25;
int led2 = 26;
int buttonPin = 33;

int ScreenSda = 35;
int ScreenScl = 32;

int licht = analogRead(32);


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTTYPE DHT11
#define DHTPIN 27
DHT dht(DHTPIN,DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define NUMFLAKES 10

float hum = dht.readHumidity();
float temp = dht.readTemperature();

/*
    Funktionen
                */

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
  delay(250);
}

void setup() {
  // Beide Pins als Ausgänge einstellen
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  dht.begin();
  Serial.begin(9600);
  Serial.println("Programm Gestartet.");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println("Fehler beim initialisieren des Displays");
    for(;;);
  }

  server.on("/temperature", []() {
      server.send(200, "text/plain", String(temp));
  });
  server.begin();

  display.clearDisplay();
  showWelcomePage();
  delay(5000);
  Serial.println(WiFi.localIP());
}

void showtempPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Temperature: ");
  display.print(temp);
  display.println("°C");
  display.println("Humidity: ");
  display.print(hum);
  display.print("%");
  display.display();
}

void flash_light() {
      digitalWrite(led1, HIGH);
      delay(25);
      digitalWrite(led1 ,LOW);
      digitalWrite(led2, HIGH);
      delay(25);
      digitalWrite(led2, LOW);
      digitalWrite(led1, HIGH);
      delay(10);
      digitalWrite(led1, LOW);
}

void loop() {
  

  if (digitalRead(buttonPin) == LOW) {
    if (lastState == HIGH) {
        lastState = LOW;
        ++page;
        Serial.println("Button pressed");
        display.clearDisplay();
        display.display();
        flash_light();
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.status());
    }
      // Serial.println(lastState);
  } else {
    lastState = HIGH;
    // Serial.println(lastState);
  }
  if (page >= 3) page = 0;
  if (page == 0) {
      showWelcomePage();
  } else if (page == 1) {
      Showlightpage();
  } else if (page == 2) {
    showtempPage();
  }
}