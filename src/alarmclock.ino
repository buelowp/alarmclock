#include "adafruit-led-backpack.h"

#define CST_OFFSET          -6
#define DST_OFFSET          (CST_OFFSET + 1)
#define TIME_BASE_YEAR		2021
#define APP_VERSION			8

#define ONE_MINUTE          (1000 * 60)

STARTUP(WiFi.selectAntenna(ANT_INTERNAL));

const uint8_t _usDSTStart[22] = { 14,13,12,10, 9, 8,14,12,11,10, 9,14,13,12,11, 9};
const uint8_t _usDSTEnd[22]   = { 7, 6, 5, 3, 2, 1, 7, 5, 4, 3, 2, 7, 6, 5, 4, 2};

bool g_timeSync;
int g_timeZone;
int g_brightness;
int g_appId;

Adafruit_7segment matrix1 = Adafruit_7segment();
SerialLogHandler logHandler;

int currentTimeZone()
{
    g_timeZone = DST_OFFSET;
    if (Time.month() > 3 && Time.month() < 11) {
        return DST_OFFSET;
    }
    if (Time.month() == 3) {
        if ((Time.day() == _usDSTStart[Time.year() - TIME_BASE_YEAR]) && Time.hour() >= 2)
            return DST_OFFSET;
        if (Time.day() > _usDSTStart[Time.year() - TIME_BASE_YEAR])
            return DST_OFFSET;
    }
    if (Time.month() == 11) {
        if ((Time.day() == _usDSTEnd[Time.year() - TIME_BASE_YEAR]) && Time.hour() <=2)
            return DST_OFFSET;
        if (Time.day() < _usDSTEnd[Time.year() - TIME_BASE_YEAR])
            return DST_OFFSET;
    }
    g_timeZone = CST_OFFSET;
    return CST_OFFSET;
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

int clockUpdateBrightness(String b)
{
    int bright;

    bright = b.toInt();

    if (bright <= 255 && bright >= 0)
        g_brightness = bright;

    matrix1.setBrightness(g_brightness);
    return g_brightness;
}

void setup()
{
    g_appId = APP_VERSION;

    Particle.function("setBright", clockUpdateBrightness);
    Particle.variable("brightness", g_brightness);
    Particle.variable("timezone", g_timeZone);
    Particle.variable("version", g_appId);
    matrix1.begin(0x70);
    clearDisplay();

    Time.zone(currentTimeZone());
    g_brightness = 5;

    matrix1.setBrightness(g_brightness);
    Log.info("Done with setup");
}

void loop()
{
    static int lastHour = 25;

    if (Time.hour() != lastHour) {
        Particle.syncTime();
        waitUntil(Particle.syncTimeDone);
        Time.zone(currentTimeZone());
        lastHour = Time.hour();
    }

    writeTime();
}
