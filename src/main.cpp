#include <Arduino.h>
#include <iostream>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <wifi_credentials.h>
#include <WebServer.h>
#include <DNSServer.h>
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
#define DHTPIN 25
DHT dht(DHTPIN,DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define NUMFLAKES 10

float hum = dht.readHumidity();
float temp = dht.readTemperature();

const char* AP_SSID = "ESP32-Setup Praktikum";
const char* AP_PASS = "etistinkt";

/*
    Funktionen
                */

const char HTML_INDEX[] PROGMEM = R"HTML(
<!DOCTYPE html><html lang="de"><meta charset="utf-8">
<title>ESP32 WLAN Setup</title>
<style>body{font-family:sans-serif;margin:2rem}input{padding:.5rem;margin:.3rem 0;width:100%}button{padding:.6rem 1rem}</style>
<h2>WLAN verbinden</h2>
<form method="POST" action="/connect">
  <label>SSID</label><br>
  <input name="ssid" placeholder="WLAN-Name" required><br>
  <label>Passwort</label><br>
  <input name="pass" type="password" placeholder="WLAN-Passwort"><br><br>
  <button type="submit">Verbinden</button>
</form>
<p>Diese Seite erreichst du unter <b>http://192.168.4.1</b></p>
)HTML";

void addCORS() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void handleOptions() {
  addCORS();
  server.send(204);
}

void handleStatus() {
  addCORS();
  server.send(200, "application/json", "{\"ok\":true}");
}

void handleTemperature() {
  addCORS();
  float t = dht.readTemperature();  // °C
  server.send(200, "text/plain", String(t));
}

void handleRoot() {
  server.send(200, "text/html", HTML_INDEX);
}

void handleConnect() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");

  server.send(200, "text/html",
              "<html><meta charset='utf-8'><body>"
              "<h3>Verbinde mit WLAN...</h3>"
              "<p>SSID: " + ssid + "</p>"
              "<p>Bitte 5–10 Sekunden warten.</p>"
              "</body></html>");

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  unsigned long start = millis();
  const unsigned long timeout = 15000;

  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Verbunden mit: " + ssid);
    Serial.print("IP im Heimnetz: "); Serial.println(WiFi.localIP());

    // Optional: AP schließen, damit nur noch Heimnetz genutzt wird
    WiFi.softAPdisconnect(true);

  } else {
    Serial.println("Verbindung fehlgeschlagen.");
  }
}

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
  Serial.begin(9600);
  pinMode(25, INPUT);
  dht.begin();
  delay(200);

  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(AP_SSID, AP_PASS);
  IPAddress apIP = WiFi.softAPIP();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println("Fehler beim initialisieren des Displays");
    for(;;);
  }

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backwardButton, INPUT_PULLUP);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println(ok ? "AP gestartet." : "AP-Start FEHLER!");
  Serial.print("AP SSID: "); Serial.println(AP_SSID);
  Serial.print("AP IP:   "); Serial.println(apIP);

  server.on("/status", HTTP_GET, handleStatus);
  server.on("/status", HTTP_OPTIONS, handleOptions);

  server.on("/temperature", HTTP_GET, handleTemperature);
  server.on("/temperature", HTTP_OPTIONS, handleOptions);


  server.on("/temperature", handleTemperature);
  server.on("/login", handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.onNotFound([](){ 
    server.send(404, "text/plain", "Not found"); 
    if (server.method() == HTTP_OPTIONS) { handleOptions(); return; }
    addCORS();
  });

  server.begin();
  Serial.println("HTTP-Server bereit.");


  display.clearDisplay();
  showWelcomePage();
}

void showtempPage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Temperatur: ");
  display.print(temp);
  display.println(" Grad C");
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

  server.handleClient();

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