#include "adafruit-led-backpack.h"

#define WLAN_SSID            "LivingRoom"
#define WLAN_PASS            "Motorazr2V8"
#define CST_OFFSET        -6
#define TIME_BASE_YEAR    2017

bool g_timeSync;
bool g_timeZoneCheck;

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

Adafruit_7segment matrix1 = Adafruit_7segment();

int currentTimeZone()
{
  int offset = CST_OFFSET;

    if (Time.month() > 3 && Time.month() < 11) {
        Time.beginDST();
    }
    else if (Time.month() == 3) {
        if ((Time.day() == _usDSTStart[Time.year() -  TIME_BASE_YEAR]) && Time.hour() >= 2)
            Time.beginDST();
        else if (Time.day() > _usDSTStart[Time.year() -  TIME_BASE_YEAR])
          Time.beginDST();
    }
    else if (Time.month() == 11) {
        if ((Time.day() == _usDSTEnd[Time.year() -  TIME_BASE_YEAR]) && Time.hour() <=2)
          Time.beginDST();
        else if (Time.day() > _usDSTEnd[Time.year() -  TIME_BASE_YEAR])
          Time.endDST();
    }

    String debug(String(__FUNCTION__) + ": Current timezone is " + offset);
    Serial.println(debug);
    return offset;
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

void setup()
{
  Serial.begin(115200);
  matrix1.begin(0x70);
  matrix1.setBrightness(10);
  clearDisplay();

  Particle.syncTime();
  Time.zone(CST_OFFSET);
  currentTimeZone();
  lostConnection();
  g_timeSync = true;
  g_timeZoneCheck = false;
  Serial.println("Done with setup");
}

void loop()
{
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

  writeTime();
}
