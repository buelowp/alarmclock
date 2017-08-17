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
//Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();
int lastMinLow;

const uint8_t _usDSTStart[20] = {12,11,10, 8,14,13,12,10,9,8,14,12,11,10,9,14,13,12,11, 9};
const uint8_t _usDSTEnd[20] = {5,4,3,1,7,6,5,3,2,1,7,5,4,3,2,7,6,5,4,2};

int count = 0;

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

static const uint8_t digitzero[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100110,
  B00101010,
  B10110010,
  B00100010,
  B00011100
};

static const uint8_t digitone[] = {
  B00000000,
  B00000100,
  B00001100,
  B10000100,
  B00000100,
  B10000100,
  B00000100,
  B00001110
};

static const uint8_t digittwo[] = {
  B00000000,
  B00011100,
  B00100010,
  B10000010,
  B00011100,
  B10100000,
  B00100000,
  B00011110
};

static const uint8_t digitthree[] = {
  B00000000,
  B00111110,
  B00000010,
  B10000100,
  B00001100,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitfour[] = {
  B00000000,
  B00000100,
  B00001100,
  B10010100,
  B00100100,
  B10111110,
  B00000100,
  B00000100
};

static const uint8_t digitfive[] = {
  B00000000,
  B00111110,
  B00100000,
  B10111100,
  B00100010,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitsix[] = {
  B00000000,
  B00001110,
  B00010000,
  B10100000,
  B00111100,
  B10100010,
  B00100010,
  B00011100
};
static const uint8_t digitseven[] = {
  B00000000,
  B00111110,
  B00000010,
  B10000010,
  B00100100,
  B10001000,
  B00010000,
  B00100000
};

static const uint8_t digiteight[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100010,
  B00011100,
  B10100010,
  B00100010,
  B00011100
};

static const uint8_t digitnine[] = {
  B00000000,
  B00011100,
  B00100010,
  B10100010,
  B00011110,
  B10000010,
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

void writeDead()
{
  matrix1.setCursor(0, 0);
  matrix1.print("D");
  matrix2.setCursor(0, 0);
  matrix2.print("E");
  matrix3.setCursor(0, 0);
  matrix3.print("A");
  /*
  matrix4.setCursor(0, 0);
  matrix4.print("D");
  */
}

void clearDisplay()
{
  matrix1.clear();
  matrix2.clear();
  matrix3.clear();
  matrix1.writeDisplay();
  matrix2.writeDisplay();
  matrix3.writeDisplay();
/*
  matrix4.clear();
  matrix4.writeDisplay();  
  */
}

void setMinHighBitmap(int index)
{
  switch (index) {
    case 0:
      matrix1.drawBitmap(0,0,digitzero,8,8,LED_ON);
      break;
    case 1:
      matrix1.drawBitmap(0,0,digitone,8,8,LED_ON);
      break;
    case 2:
      matrix1.drawBitmap(0,0,digittwo,8,8,LED_ON);
      break;
    case 3:
      matrix1.drawBitmap(0,0,digitthree,8,8,LED_ON);
      break;
    case 4:
      matrix1.drawBitmap(0,0,digitfour,8,8,LED_ON);
      break;
    case 5:
      matrix1.drawBitmap(0,0,digitfive,8,8,LED_ON);
      break;
    case 6:
      matrix1.drawBitmap(0,0,digitsix,8,8,LED_ON);
      break;
    case 7:
      matrix1.drawBitmap(0,0,digitseven,8,8,LED_ON);
      break;
    case 8:
      matrix1.drawBitmap(0,0,digiteight,8,8,LED_ON);
      break;
    case 9:
      matrix1.drawBitmap(0,0,digitnine,8,8,LED_ON);
      break;
  }
}

void writeTime()
{
  int minlow = (minute() % 10);
  int minhigh = (minute() / 10) % 10;
  int newhour = hour();
  if (newhour > 12)
    newhour = newhour - 12;
    
  int hourlow = (newhour % 10);
  int hourhigh = (newhour / 10) % 10;
  
  if (lastMinLow != minlow) {
    clearDisplay();
    matrix1.setCursor(0,0);
    matrix2.setCursor(2,1);
    matrix3.setCursor(0,1);

    matrix1.setRotation(3);
    matrix2.setRotation(3);
    matrix3.setRotation(3);
    
    matrix3.print(minlow);
    setMinHighBitmap(minhigh);
    matrix2.print(hourlow);

    matrix1.writeDisplay();
    matrix2.writeDisplay();
    matrix3.writeDisplay();
    lastMinLow = minlow;
  }

/*
  matrix4.setCursor(0,0);
  matrix4.print(minlow);

  matrix4.writeDisplay();  

  Serial.print(count++);
  Serial.print(": ");
  Serial.print(hourhigh);
  Serial.print(hourlow);
  Serial.print(":");
  Serial.print(minhigh);
  Serial.println(minlow);
*/
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("Starting");
  
  matrix1.begin(0x70);
  matrix2.begin(0x71);
  matrix3.begin(0x76);
  matrix1.setTextSize(1);
  matrix2.setTextSize(1);
  matrix3.setTextSize(1);
  matrix1.setTextColor(LED_ON);
  matrix2.setTextColor(LED_ON);
  matrix3.setTextColor(LED_ON);
/*
  matrix4.begin(0x73);
  matrix4.setTextSize(1);
  matrix4.setTextColor(LED_ON);
*/  
  clearDisplay();
  writeDead();
 
  WiFi.persistent(false);
  WiFi.begin("Office", "Motorazr2V8");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
    Serial.println("Got NTP event");
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

  pinMode(BUILTIN_LED, OUTPUT);  // set onboard LED as output
  lastMinLow = 99;
}

void loop() 
{
  writeTime();
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  delay(100);
}
