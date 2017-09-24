
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_LEDBackpack.h>
#include <Time.h>
#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <NtpClientLib.h>

#include "clockbitmaps.h"

#define WLAN_SSID            "LivingRoom"
#define WLAN_PASS            "Motorazr2V8"
#define CST_OFFSET        -6
#define DST_OFFSET        (CST_OFFSET + 1)
#define TIME_BASE_YEAR    2017
#define ONBOARDLED 2 // Built in LED on ESP-12/ESP-07

WiFiUDP ntpUDP;

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

int lastMinLow;
int lastMinHigh;
int lastHourLow;
int lastHourHigh;
int count;
bool g_on;
bool g_timeSet;
boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

// Start NTP only after IP network is connected
void onSTAGotIP(WiFiEventStationModeGotIP ipInfo) 
{
  Serial.printf("Got IP: %s\r\n", ipInfo.ip.toString().c_str());
  NTP.begin("pool.ntp.org", currentTimeZone(), false);
  NTP.setInterval(60*60);
}

// Manage network disconnection
void onSTADisconnected(WiFiEventStationModeDisconnected event_info) 
{
  Serial.printf("Disconnected from SSID: %s\n", event_info.ssid.c_str());
  Serial.printf("Reason: %d\n", event_info.reason);
  NTP.stop(); // NTP sync can be disabled to avoid sync errors
}

void processSyncEvent(NTPSyncEvent_t ntpEvent) 
{
  if (ntpEvent) {
    Serial.print("Time Sync error: ");
    if (ntpEvent == noResponse)
      Serial.println("NTP server not reachable");
    else if (ntpEvent == invalidAddress)
      Serial.println("Invalid NTP server address");
  }
  else {
    Serial.print("Got NTP time: ");
    Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
  }
  NTP.setTimeZone(currentTimeZone());
}

int currentTimeZone()
{
  int offset = CST_OFFSET;

    if (month() > 3 && month() < 11) {
        offset = DST_OFFSET;
    }
    else if (month() == 3) {
        if ((day() == _usDSTStart[year() -  TIME_BASE_YEAR]) && hour() >= 2)
            offset = DST_OFFSET;
        else if (day() > _usDSTStart[year() -  TIME_BASE_YEAR])
            offset = DST_OFFSET;
    }
    else if (month() == 11) {
        if ((day() == _usDSTEnd[year() -  TIME_BASE_YEAR]) && hour() <=2)
            offset = DST_OFFSET;
        else if (day() > _usDSTEnd[year() -  TIME_BASE_YEAR])
            offset = CST_OFFSET;
    }

    String debug(String(__FUNCTION__) + ": Current timezone is " + offset);
    Serial.println(debug);
    return offset;
}

void setMinLowBitmap(int index)
{
  switch (index) {
    case 0:
      matrix4.drawBitmap(0,0,digitzerolow,8,8,LED_ON);
      break;
    case 1:
      matrix4.drawBitmap(0,0,digitonelow,8,8,LED_ON);
      break;
    case 2:
      matrix4.drawBitmap(0,0,digittwolow,8,8,LED_ON);
      break;
    case 3:
      matrix4.drawBitmap(0,0,digitthreelow,8,8,LED_ON);
      break;
    case 4:
      matrix4.drawBitmap(0,0,digitfourlow,8,8,LED_ON);
      break;
    case 5:
      matrix4.drawBitmap(0,0,digitfivelow,8,8,LED_ON);
      break;
    case 6:
      matrix4.drawBitmap(0,0,digitsixlow,8,8,LED_ON);
      break;
    case 7:
      matrix4.drawBitmap(0,0,digitsevenlow,8,8,LED_ON);
      break;
    case 8:
      matrix4.drawBitmap(0,0,digiteightlow,8,8,LED_ON);
      break;
    case 9:
      matrix4.drawBitmap(0,0,digitninelow,8,8,LED_ON);
      break;
  }
}

void setMinHighBitmap(int index)
{
  switch (index) {
    case 0:
      matrix3.drawBitmap(0,0,digitzerocolon,8,8,LED_ON);
      break;
    case 1:
      matrix3.drawBitmap(0,0,digitonecolon,8,8,LED_ON);
      break;
    case 2:
      matrix3.drawBitmap(0,0,digittwocolon,8,8,LED_ON);
      break;
    case 3:
      matrix3.drawBitmap(0,0,digitthreecolon,8,8,LED_ON);
      break;
    case 4:
      matrix3.drawBitmap(0,0,digitfourcolon,8,8,LED_ON);
      break;
    case 5:
      matrix3.drawBitmap(0,0,digitfivecolon,8,8,LED_ON);
      break;
    case 6:
      matrix3.drawBitmap(0,0,digitsixcolon,8,8,LED_ON);
      break;
    case 7:
      matrix3.drawBitmap(0,0,digitsevencolon,8,8,LED_ON);
      break;
    case 8:
      matrix3.drawBitmap(0,0,digiteightcolon,8,8,LED_ON);
      break;
    case 9:
      matrix3.drawBitmap(0,0,digitninecolon,8,8,LED_ON);
      break;
  }
}

void setHourLowBitmap(int index)
{
  switch (index) {
    case 1:
      matrix2.drawBitmap(0,0,digitone,8,8,LED_ON);
      break;
    case 2:
      matrix2.drawBitmap(0,0,digittwo,8,8,LED_ON);
      break;
    case 3:
      matrix2.drawBitmap(0,0,digitthree,8,8,LED_ON);
      break;
    case 4:
      matrix2.drawBitmap(0,0,digitfour,8,8,LED_ON);
      break;
    case 5:
      matrix2.drawBitmap(0,0,digitfive,8,8,LED_ON);
      break;
    case 6:
      matrix2.drawBitmap(0,0,digitsix,8,8,LED_ON);
      break;
    case 7:
      matrix2.drawBitmap(0,0,digitseven,8,8,LED_ON);
      break;
    case 8:
      matrix2.drawBitmap(0,0,digiteight,8,8,LED_ON);
      break;
    case 9:
      matrix2.drawBitmap(0,0,digitnine,8,8,LED_ON);
      break;
    case 0:
    case 10:
      matrix2.drawBitmap(0,0,digitzero,8,8,LED_ON);
      matrix1.drawBitmap(0,0,digitoneshifted,8,8,LED_ON);
      break;
    case 11:
      matrix2.drawBitmap(0,0,digitone,8,8,LED_ON);
      matrix1.drawBitmap(0,0,digitoneshifted,8,8,LED_ON);
      break;
    case 12:
      matrix2.drawBitmap(0,0,digittwo,8,8,LED_ON);
      matrix1.drawBitmap(0,0,digitoneshifted,8,8,LED_ON);
      break;
  }
}

void clearDisplay()
{
  matrix1.clear();
  matrix2.clear();
  matrix3.clear();
  matrix4.clear();
  matrix1.writeDisplay();
  matrix2.writeDisplay();
  matrix3.writeDisplay();
  matrix4.writeDisplay();  
}

void writeTime()
{
  int minLow = (minute() % 10);
  int minHigh = (minute() / 10) % 10;
  int hourLow = 0;
  int hourHigh = 0;
  int newhour = hour();

  if (newhour > 12)
    newhour = newhour - 12;
    
  hourLow = (newhour % 10);
  hourHigh = (newhour / 10) % 10;

  if (lastMinLow != minLow) {
    matrix4.clear();
    setMinLowBitmap(minLow);
    matrix4.writeDisplay();
    lastMinLow = minLow;
  }
  if (lastMinHigh != minHigh) {
    matrix3.clear();
    setMinHighBitmap(minHigh);
    matrix3.writeDisplay();
    lastMinHigh = minHigh;
    lostConnection();
  }
  if (lastHourLow != hourLow) {
    matrix2.clear();
    matrix1.clear();
    setHourLowBitmap(newhour);
    matrix2.writeDisplay();
    matrix1.writeDisplay();
    lastHourLow = hourLow;
    if (hourHigh == 0) {
      matrix1.clear();
      if (WiFi.status() == WL_CONNECTED) {
        matrix1.drawBitmap(0,0,smile_bmp,8,8,LED_ON);
      }
      else {
        matrix1.drawBitmap(0,0,frown_bmp,8,8,LED_ON);        
      }
      matrix1.writeDisplay();
    }
  }
}

void lostConnection()
{
  Serial.print(count++);
  Serial.print(": ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(", ");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print("-");
  Serial.println(year());
}

void setup() 
{
  static WiFiEventHandler e1, e2;

  Serial.begin(115200);
  matrix1.begin(0x71);
  matrix2.begin(0x72);
  matrix3.begin(0x75);
  matrix4.begin(0x74);
  matrix1.setRotation(3);
  matrix2.setRotation(3);
  matrix3.setRotation(3);
  matrix4.setRotation(3);
  matrix1.setTextSize(1);
  matrix2.setTextSize(1);
  matrix3.setTextSize(1);
  matrix4.setTextSize(1);
  matrix1.setTextColor(LED_ON);
  matrix2.setTextColor(LED_ON);
  matrix3.setTextColor(LED_ON);
  matrix4.setTextColor(LED_ON);

  WiFi.begin("LivingRoom", "Motorazr2V8");

  NTP.setTimeZone(CST_OFFSET);
  NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
    ntpEvent = event;
    syncEventTriggered = true;
  });

  e1 = WiFi.onStationModeGotIP(onSTAGotIP);// As soon WiFi is connected, start NTP Client
  e2 = WiFi.onStationModeDisconnected(onSTADisconnected);

  clearDisplay();

  lastMinLow = 99;
  lastMinHigh = 99;
  lastHourLow = 99;
  lastHourHigh = 99;
  count = 0;
  g_on = false;
  g_timeSet = true;
  delay(2000);
  Serial.println("Done with setup");
}

void loop() 
{
  if (syncEventTriggered) {
    processSyncEvent(ntpEvent);
    syncEventTriggered = false;
  }

  writeTime();
  delay(0);
}

