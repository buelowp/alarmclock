#include "adafruit-led-backpack.h"

#define WLAN_SSID            "LivingRoom"
#define WLAN_PASS            "Motorazr2V8"
#define CST_OFFSET        -6
#define DST_OFFSET        (CST_OFFSET + 1)
#define TIME_BASE_YEAR    2017

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

Adafruit_7segment matrix1 = Adafruit_7segment();

int currentTimeZone()
{
  int offset = CST_OFFSET;

    if (Time.month() > 3 && Time.month() < 11) {
        offset = DST_OFFSET;
    }
    else if (Time.month() == 3) {
        if ((Time.day() == _usDSTStart[Time.year() -  TIME_BASE_YEAR]) && Time.hour() >= 2)
            offset = DST_OFFSET;
        else if (Time.day() > _usDSTStart[Time.year() -  TIME_BASE_YEAR])
            offset = DST_OFFSET;
    }
    else if (Time.month() == 11) {
        if ((Time.day() == _usDSTEnd[Time.year() -  TIME_BASE_YEAR]) && Time.hour() <=2)
            offset = DST_OFFSET;
        else if (Time.day() > _usDSTEnd[Time.year() -  TIME_BASE_YEAR])
            offset = CST_OFFSET;
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
  matrix1.setBrightness(15);
  clearDisplay();

  Serial.println("Done with setup");
}

void loop()
{
  writeTime();
  delay(0);
}
