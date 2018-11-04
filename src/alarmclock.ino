#include "adafruit-led-backpack.h"
#include <photon-wdgs.h>
#include <SparkIntervalTimer.h>

#define WLAN_SSID           "LivingRoom"
#define WLAN_PASS           "Motorazr2V8"
#define CST_OFFSET          -6
#define TIME_BASE_YEAR      2017
#define APP_VERSION			4

#define ONE_MINUTE          (1000 * 60)

STARTUP(WiFi.selectAntenna(ANT_INTERNAL));
//PRODUCT_VERSION(APP_VERSION);

int clockUpdateBrightness(String);

bool g_timeSync;
bool g_timeZoneCheck;
int g_brightness;
int g_appId;

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

Adafruit_7segment matrix1 = Adafruit_7segment();

void currentTimeZone()
{
    if (Time.month() > 3 && Time.month() < 11) {
        Time.beginDST();
    }
    else if (Time.month() == 3) {
        if (Time.day() < (_usDSTStart[Time.year() - TIME_BASE_YEAR]))
            Time.endDST();
        else if ((Time.day() == _usDSTStart[Time.year() -  TIME_BASE_YEAR]) && Time.hour() < 2)
            Time.endDST();
        else if ((Time.day() == _usDSTStart[Time.year() -  TIME_BASE_YEAR]) && Time.hour() >= 2)
            Time.beginDST();
        else
            Time.beginDST();
    }
    else if (Time.month() == 11) {
        if (Time.day() < (_usDSTEnd[Time.year() - TIME_BASE_YEAR]))
            Time.beginDST();
        else if ((Time.day() == _usDSTEnd[Time.year() -  TIME_BASE_YEAR]) && Time.hour() <= 2)
            Time.beginDST();
        else if ((Time.day() == _usDSTEnd[Time.year() -  TIME_BASE_YEAR]) && Time.hour() > 2)
            Time.endDST();
        else
            Time.endDST();
    }
}

void clearDisplay()
{
    matrix1.clear();
    matrix1.writeDisplay();
}

void writeTime()
{
  // Show the time on the display by turning it into a numeric
  // value, like 3:30 turns into 330, by multiplying the hour by
  // 100 and then adding the minutes.
  int displayValue = Time.hour() * 100 + Time.minute();

  // Handle when hours are past 12 by subtracting 12 hours (1200 value).
  if (Time.hour() > 12) {
    displayValue -= 1200;
  }
  if (Time.hour() == 0) {
      displayValue += 1200;
  }

  matrix1.print(displayValue, DEC);
  matrix1.drawColon(true);
  matrix1.writeDisplay();
}

void lostConnection()
{
  static int count = 0;

  Serial.print(count++);
  Serial.print(": ");
  Serial.print(Time.hour());
  Serial.print(":");
  Serial.print(Time.minute());
  Serial.print(", ");
  Serial.print(Time.month());
  Serial.print("-");
  Serial.print(Time.day());
  Serial.print("-");
  Serial.println(Time.year());
}

int clockUpdateBrightness(String b)
{
    int bright;

    bright = b.toInt();

    if (bright <= 15 && bright >= 0)
        g_brightness = bright;

    return g_brightness;
}

void setup()
{
    Serial.begin(115200);
    g_appId = APP_VERSION;

    Particle.function("setBright", clockUpdateBrightness);
    Particle.variable("brightness", g_brightness);
    Particle.variable("version", g_appId);
    matrix1.begin(0x70);
    matrix1.setBrightness(5);
    clearDisplay();

    Particle.syncTime();
    Time.zone(CST_OFFSET);
    currentTimeZone();
    lostConnection();
    g_timeSync = true;
    g_timeZoneCheck = false;
    g_brightness = 5;

    PhotonWdgs::begin(true, true, ONE_MINUTE, TIMER7);

    Serial.println("Done with setup");
}

void loop()
{
    if (WiFi.ready()) {
        PhotonWdgs::tickle();
    }

    /* Call network time 4 times a day */
    if (((Time.hour() % 6) == 0) && (!g_timeSync)) {
        Particle.syncTime();
        g_timeSync = true;
        Serial.println("Asked the network for time");
    }
    else
        g_timeSync = false;

    /* Need to update the timezone every hour for the 2 times it matters a year */
    if ((Time.minute() == 1) && g_timeZoneCheck) {
        currentTimeZone();
        Serial.println("Updated the timezone");
        g_timeZoneCheck = false;
    }
    else {
        g_timeZoneCheck = true;
    }

    matrix1.setBrightness(g_brightness);
    writeTime();
}
