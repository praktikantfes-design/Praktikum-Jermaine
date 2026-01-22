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

int lastState1 = HIGH;
int currentState1 = HIGH;
int lastState2 = HIGH;
int currentState2 = HIGH;
int page = 0;
bool toggled = false;
bool ledAn = false;

int led1 = 25;
int led2 = 26;
int forwardButton = 33;
int backwardButton = 32;

int ScreenSda = 21;
int ScreenScl = 22;

int licht = analogRead(34);


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTTYPE DHT11
#define DHTPIN 35
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
  display.setCursor(50, 20);
  display.setTextSize(1.7);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Wilkommen (1)");
  display.setTextSize(0.8);
  display.println("Geraete-IP: ");
  display.println(WiFi.localIP());
  display.println("Wlan Status: ");
  if (WiFi.status() == WL_CONNECTED) {
    display.println("Verbunden");
  } else {
    display.println("Getrennt");
  }
  display.display();
}


void Showlightpage() {
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Helligkeit (2)");
  display.setTextSize(1.3);
  display.print(map(analogRead(34), 0, 4095, 0, 100));
  display.println("%");
  display.setTextSize(1);
  display.print(analogRead(34));
  display.print(" (0-4095)");
  display.display();
  delay(250);
}

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backwardButton, INPUT_PULLUP);
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
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Verbunden");
  }
}

void showtempPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Temperatur: ");
  display.print(temp);
  display.println("°C");
  display.println("Luftfeuchtigkeit: ");
  display.print(hum);
  display.print("%");
  display.display();
}

void flash_light() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  delay(25);
  digitalWrite(led1 ,LOW);
  digitalWrite(led2, LOW);

}

void loop() {

  lastState1 = currentState1;
  currentState1 = digitalRead(33);

  lastState2 = currentState2;
  currentState2 = digitalRead(32);

  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);

  if (currentState1 != lastState1) {
    if (currentState1 == LOW) {
      ++page;
      flash_light();
      // Serial.println(page);
      Serial.println("Fwd");
      delay(50);
    }
    
  }

  if (currentState2 != lastState2) {
    if (currentState2 == LOW) {
      --page;
      flash_light();
      // Serial.println(page);
      Serial.println("Bwd");
      delay(50);
    }
    
  }

  if (page == 0) {
    showWelcomePage();
  } else if (page == 1) {
    Showlightpage();
  } else if (page == 2) {
    showtempPage();
  } else if (page == 3) {
    page = 0;
  } else if (page <= 0) {
    page = 2;
  }
}