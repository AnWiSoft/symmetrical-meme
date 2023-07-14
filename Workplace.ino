/***********************************************************************************
 *  Arbeitsplatzsteuerung nach Bodmer TFT_eSPI Lib
 *  10.07.2023
 *********************************************************************************/
#include <Arduino.h>
#include "WiFi.h"
#include "fauxmoESP.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Raspi Treiber 20 MHz einstllen

TFT_eSPI tft = TFT_eSPI();       

#define WIFI_SSID "WLAN-QRAT68"
#define WIFI_PASS "2102296026106923"
IPAddress apIP(192, 168, 2, 250);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 2, 1);

int F_X[] ={20, 240, 20, 240, 20};
int F_Y[] ={20, 20, 120, 120, 220};
// Pin      14,  27, 26, 25, 33
bool SwitchOn[] ={LOW, LOW, LOW, LOW, LOW};

#define F_W 200
#define F_H 80
#define RELAY_PIN_1 14
#define RELAY_PIN_2 27
#define RELAY_PIN_3 26
#define RELAY_PIN_4 25
#define RELAY_PIN_5 33

#define SCHALTER_1 "Ladegeraet"
#define SCHALTER_2 "Picoskop"
#define SCHALTER_3 "Stromversorger"
#define SCHALTER_4 "Werkzeug"
#define SCHALTER_5 "Bedienungsanleitung"

fauxmoESP fauxmo;

void wifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.config(apIP, gateway, subnet, dns);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1500);
     }
  // Serial.println("WiFi gefunden!");
         delay(500);  
  // Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup(void)
{
  //Serial.begin(115200);
  pinMode(RELAY_PIN_1, OUTPUT); digitalWrite(RELAY_PIN_1, LOW);
  pinMode(RELAY_PIN_2, OUTPUT); digitalWrite(RELAY_PIN_2, LOW);
  pinMode(RELAY_PIN_3, OUTPUT); digitalWrite(RELAY_PIN_3, LOW);
  pinMode(RELAY_PIN_4, OUTPUT); digitalWrite(RELAY_PIN_4, LOW);
  pinMode(RELAY_PIN_5, OUTPUT); digitalWrite(RELAY_PIN_5, LOW);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLUE);
for(int x=0; x<5;x++){
  redBtn(F_X[x], F_Y[x], x);}
  wifiSetup();
  fauxmo.createServer(true); 
  fauxmo.setPort(80); 
  fauxmo.enable(true);
  
  fauxmo.addDevice(SCHALTER_1);
  fauxmo.addDevice(SCHALTER_2);
  fauxmo.addDevice(SCHALTER_3);
  fauxmo.addDevice(SCHALTER_4);
  fauxmo.addDevice(SCHALTER_5);
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    // Callback when a command from Alexa is received.

// Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, SCHALTER_1) == 0) ) {
      //Serial.println("RELAY 1 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);
         SwitchOn[0] = HIGH;greenBtn(F_X[0], F_Y[0], 0);
      } else {
        digitalWrite(RELAY_PIN_1, LOW);
         SwitchOn[0] = LOW;redBtn(F_X[0], F_Y[0], 0);
      }
    }
     if ( (strcmp(device_name, SCHALTER_2) == 0) ) {
     // Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, HIGH);
        SwitchOn[1] = HIGH; greenBtn(F_X[1], F_Y[1], 1);
      } else {
        digitalWrite(RELAY_PIN_2, LOW);
         SwitchOn[1] = LOW; redBtn(F_X[1], F_Y[1], 1);
      }
    }
     if ( (strcmp(device_name, SCHALTER_3) == 0) ) {
      //Serial.println("RELAY 3 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_3, HIGH);
        SwitchOn[2] = HIGH; greenBtn(F_X[2], F_Y[2], 2);
      } else {
        digitalWrite(RELAY_PIN_3, LOW);
         SwitchOn[2] = LOW; redBtn(F_X[2], F_Y[2], 2);
      }
    }
     if ( (strcmp(device_name, SCHALTER_4) == 0) ) {
     // Serial.println("RELAY 4 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_4, HIGH);
        SwitchOn[3] = HIGH; greenBtn(F_X[3], F_Y[3], 3);
      } else {
        digitalWrite(RELAY_PIN_4, LOW);
         SwitchOn[3] = LOW; redBtn(F_X[3], F_Y[3], 3);
      }
    }
    if ( (strcmp(device_name, SCHALTER_5) == 0) ) {
     //Serial.println("RELAY 5 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_5, HIGH);
        SwitchOn[4] = HIGH; greenBtn(F_X[4], F_Y[4], 4);
      } else {
        digitalWrite(RELAY_PIN_5, LOW);
         SwitchOn[4] = LOW;  redBtn(F_X[4], F_Y[4], 4);
      }
    }
});
}
//------------------------------------------------------------------------------------------
void loop()
{
  uint16_t x, y;
 fauxmo.handle();

  // See if there's any touch data for us
 if (tft.getTouch(&x, &y))
  {
    // Draw a block spot to show where touch was calculated to be
         tft.fillCircle(x, y, 2, TFT_BLACK);
for(int i=0; i<5;i++){    
    if (SwitchOn[i])
    {
      if ((x > F_X[i]) && (x < (F_X[i] + F_W/2))) {
        if ((y > F_Y[i]) && (y <= (F_Y[i] + F_H))) {
          //Serial.println("Red btn hit");
          redBtn(F_X[i], F_Y[i], i);
          SwitchOn[i] = LOW;
          switch(i){
            case 0: digitalWrite(14, LOW);  break;
            case 1: digitalWrite(25, LOW);  break;
            case 2: digitalWrite(26, LOW);  break;
            case 3: digitalWrite(27, LOW);  break;
            case 4: digitalWrite(33, LOW);  break;
          }
          }
      }
    }
    else //Record is off (SwitchOn == false)
    {
      if ((x > F_X[i]+F_W/2) && (x < (F_X[i]+F_W))) {
        if ((y > F_Y[i]) && (y <= (F_Y[i] + F_H))) {
          //Serial.println("Green btn hit");
          greenBtn(F_X[i], F_Y[i], i);
          SwitchOn[i] = HIGH;
          switch(i){
            case 0: digitalWrite(14, HIGH);  break;
            case 1: digitalWrite(25, HIGH);  break;
            case 2: digitalWrite(26, HIGH);  break;
            case 3: digitalWrite(27, HIGH);  break;
            case 4: digitalWrite(33, HIGH);  break;
          }
          }
      }
    }
  }
  }
   /*static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }
    fauxmo.setState(SCHALTER_1, SwitchOn[0], 255);
    fauxmo.setState(SCHALTER_2, SwitchOn[1], 255);
    fauxmo.setState(SCHALTER_3, SwitchOn[2], 255);
    fauxmo.setState(SCHALTER_4, SwitchOn[3], 255);
    fauxmo.setState(SCHALTER_5, SwitchOn[4], 255);*/
 }

void drawFrame(int fx, int fy)
{
  tft.drawRect(fx, fy, F_W, F_H, TFT_BLACK);
}

// Draw a red button
void redBtn(int Fx, int Fy, int x)
{
  tft.fillRect(Fx, Fy, F_W/2, F_H, TFT_RED);
  tft.fillRect(Fx+F_W/2, Fy, F_W/2, F_H, TFT_DARKGREY);
  drawFrame(Fx, Fy);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("ON", Fx+3*F_W/4, Fy + F_H/2);
  SwitchOn[x] = LOW;
}

// Draw a green button
void greenBtn(int Fx, int Fy, int x)
{
  tft.fillRect(Fx+F_W/2, Fy, F_W/2, F_H, TFT_GREEN);
  tft.fillRect(Fx, Fy, F_W/2, F_H, TFT_DARKGREY);
  drawFrame(Fx, Fy);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", Fx + F_W / 4, Fy + (F_H / 2));
  SwitchOn[x] = HIGH;
}
