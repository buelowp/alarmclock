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
#include <Time.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#define CST_OFFSET        -6
#define DST_OFFSET        (CST_OFFSET + 1)
#define TIME_BASE_YEAR    2017

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

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

void writeDead()
{
  matrix1.setCursor(0, 0);
  matrix1.print("D");
  matrix2.setCursor(0, 0);
  matrix2.print("E");
  matrix3.setCursor(0, 0);
  matrix3.print("A");
  matrix4.setCursor(0, 0);
  matrix4.print("D");
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
  int minlow = (minute() / 10) % 10;
  int minhigh = (minute() / 100) % 10;
  int hourlow = (hour() / 10) % 10;
  int hourhigh = (hour() / 100) % 10;

  clearDisplay();
  matrix1.setCursor(0,0);
  matrix1.print(minlow);
  matrix2.setCursor(0,0);
  matrix2.print(minlow);
  matrix3.setCursor(0,0);
  matrix3.print(minlow);
  matrix4.setCursor(0,0);
  matrix4.print(minlow);

  matrix1.writeDisplay();
  matrix2.writeDisplay();
  matrix3.writeDisplay();
  matrix4.writeDisplay();  

  Serial.print(hourhigh);
  Serial.print(hourlow);
  Serial.print(":");
  Serial.print(minhigh);
  Serial.println(minlow);
}

void setup() 
{
  matrix1.begin(0x70);
  matrix2.begin(0x71);
  matrix3.begin(0x72);
  matrix4.begin(0x73);
  matrix1.setTextSize(1);
  matrix2.setTextSize(1);
  matrix3.setTextSize(1);
  matrix4.setTextSize(1);
  matrix1.setTextColor(LED_ON);
  matrix2.setTextColor(LED_ON);
  matrix3.setTextColor(LED_ON);
  matrix4.setTextColor(LED_ON);
  
  clearDisplay();
  writeDead();
  
  WiFi.persistent(false);
  WiFi.begin("LivingRoom", "Motorazr2V8");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
    if (error) {
    Serial.print("Time Sync error: ");
    if (error == noResponse)
      Serial.println("NTP server not reachable");
    else if (error == invalidAddress)
      Serial.println("Invalid NTP server address");
    }
    else {
      Serial.print("Got NTP time: ");
      Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
      NTP.setTimeZone(currentTimeZone());
    }
  });
  NTP.begin("pool.ntp.org", 1, true);
  NTP.setInterval(3600);
  NTP.setTimeZone(currentTimeZone());
}

void loop() 
{
  writeTime();
}
