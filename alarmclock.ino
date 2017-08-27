#include <adafruit_feather.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_LEDBackpack.h>
#include <Time.h>
#include <TimeLib.h>

#define WLAN_SSID            "LivingRoom"
#define WLAN_PASS            "Motorazr2V8"
#define CST_OFFSET        -6
#define DST_OFFSET        (CST_OFFSET + 1)
#define TIME_BASE_YEAR    2017

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

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 
  },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 
  },
  frown_bmp[] =
  { 
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 
  };

static const uint8_t digitoneshifted[] = {
  B00000000,
  B00000001,
  B00000011,
  B00000001,
  B00000001,
  B00000001,
  B00000001,
  B00000011
};

static const uint8_t digittwoshifted[] = {
  B00000000,
  B00011100,
  B00100010,
  B00000010,
  B00011100,
  B00100000,
  B00100000,
  B10111110
};

static const uint8_t digiteleven[] = {
  B00000000,
  B01000100,
  B11001100,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B11101110
};

static const uint8_t digitonehigh[] = {
  B00000000,
  B00000010,
  B00000110,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000111
};

static const uint8_t digitzerocolon[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100110,
  B00101010,
  B10110010,
  B00100010,
  B00011100
};

static const uint8_t digitonecolon[] = {
  B00000000,
  B00000100,
  B00001100,
  B10000100,
  B00000100,
  B10000100,
  B00000100,
  B00001110
};

static const uint8_t digittwocolon[] = {
  B00000000,
  B00011100,
  B00100010,
  B10000010,
  B00011100,
  B10100000,
  B00100000,
  B00111110
};

static const uint8_t digitthreecolon[] = {
  B00000000,
  B00111110,
  B00000010,
  B10000100,
  B00001100,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitfourcolon[] = {
  B00000000,
  B00000100,
  B00001100,
  B10010100,
  B00100100,
  B10111110,
  B00000100,
  B00000100
};

static const uint8_t digitfivecolon[] = {
  B00000000,
  B00111110,
  B00100000,
  B10111100,
  B00100010,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitsixcolon[] = {
  B00000000,
  B00001110,
  B00010000,
  B10100000,
  B00111100,
  B10100010,
  B00100010,
  B00011100
};
static const uint8_t digitsevencolon[] = {
  B00000000,
  B00111110,
  B00000010,
  B10000010,
  B00100100,
  B10001000,
  B00010000,
  B00100000
};

static const uint8_t digiteightcolon[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100010,
  B00011100,
  B10100010,
  B00100010,
  B00011100
};

static const uint8_t digitninecolon[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100010,
  B00011110,
  B10000010,
  B00000100,
  B00111000
};

static const uint8_t digitzero[] = {
  B00000000,
  B00011100,
  B00100010,
  B00100110,
  B00101010,
  B00110010,
  B00100010,
  B00011100
};

static const uint8_t digitone[] = {
  B00000000,
  B00000100,
  B00001100,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00001110
};

static const uint8_t digittwo[] = {
  B00000000,
  B00011100,
  B00100010,
  B00000010,
  B00011100,
  B00100000,
  B00100000,
  B00111110
};

static const uint8_t digitthree[] = {
  B00000000,
  B00111110,
  B00000010,
  B00000100,
  B00001100,
  B00000010,
  B00100010,
  B00011100
};

static const uint8_t digitfour[] = {
  B00000000,
  B00000100,
  B00001100,
  B00010100,
  B00100100,
  B00111110,
  B00000100,
  B00000100
};

static const uint8_t digitfive[] = {
  B00000000,
  B00111110,
  B00100000,
  B00111100,
  B00100010,
  B00000010,
  B00100010,
  B00011100
};

static const uint8_t digitsix[] = {
  B00000000,
  B00001110,
  B00010000,
  B00100000,
  B00111100,
  B00100010,
  B00100010,
  B00011100
};
static const uint8_t digitseven[] = {
  B00000000,
  B00111110,
  B00000010,
  B00000010,
  B00100100,
  B00001000,
  B00010000,
  B00100000
};

static const uint8_t digiteight[] = {
  B00000000,
  B00011100,
  B00100010,
  B00100010,
  B00011100,
  B00100010,
  B00100010,
  B00011100
};

static const uint8_t digitnine[] = {
  B00000000,
  B00011100,
  B00100010,
  B00100010,
  B00011110,
  B00000010,
  B00000100,
  B00111000
};

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
    case 0:
      matrix2.drawBitmap(0,0,digitzero,8,8,LED_ON);
      break;
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
    case 11:
      matrix2.drawBitmap(0,0,digiteleven,8,8,LED_ON);
      break;
    case 12:
      matrix2.drawBitmap(0,0,digittwoshifted,8,8,LED_ON);
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
    matrix4.setCursor(0,1);
    matrix4.print(minLow);
    matrix4.writeDisplay();
    lastMinLow = minLow;
  }
  if (lastMinHigh != minHigh) {
    matrix3.clear();
    setMinHighBitmap(minHigh);
    matrix3.writeDisplay();
    lastMinHigh = minHigh;
  }
  if (lastHourLow != hourLow) {
    setHourLowBitmap(hour());
    matrix2.writeDisplay();
    matrix1.writeDisplay();
    lastHourLow = hourLow;
  }        
  if (hour() < 10) {
      matrix1.clear();
      matrix1.drawBitmap(0,0,smile_bmp,8,8,LED_ON);
      matrix1.writeDisplay();
  }

  Serial.print(count++);
  Serial.print(": ");
  Serial.print(hourHigh);
  Serial.print(hourLow);
  Serial.print(":");
  Serial.print(minHigh);
  Serial.print(minLow);
  Serial.print(", ");
  Serial.print(month());
  Serial.print("-");
  Serial.print(day());
  Serial.print("-");
  Serial.println(year());
}

void setLocalTime()
{
  int tz = currentTimeZone();
  int tz_offset = (tz * 60 * 60);

  Serial.print("tz offset = ");
  Serial.println(tz_offset);
  setTime(Feather.getUtcTime() + tz_offset);
}

void setMicroTime()
{
  if (minute() == 0 && g_timeSet == false) {
    setLocalTime();
    g_timeSet = true;
  }
  else if (minute() != 0) {
    g_timeSet = false;
  }
}

void lostConnection()
{
  while (!Feather.connected()) {
    digitalWrite(PA15, g_on);   // turn the LED on (HIGH is the voltage level)
    g_on = !g_on;
    Feather.connect(WLAN_SSID, WLAN_PASS);
    delay(250);
  }
}

void setup() 
{
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
  
  clearDisplay();
  Feather.connect(WLAN_SSID, WLAN_PASS);
  setLocalTime();
  pinMode(PA15, OUTPUT);
  digitalWrite(PA15, 1);

  lastMinLow = 99;
  lastMinHigh = 99;
  lastHourLow = 99;
  lastHourHigh = 99;
  count = 0;
  g_on = false;
  g_timeSet = true;
  delay(2000);
  setLocalTime();
  Serial.println("Done with setup");
}

void loop() 
{
  setMicroTime();
  writeTime();
  delay(1000 * 30);
}

