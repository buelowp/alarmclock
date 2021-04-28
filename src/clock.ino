#include "sunset.h"
#include "Adafruit_LEDBackpack_RK.h"
#include "MQTT.h"

#define APP_ID              9
#define CST_OFFSET          -6
#define DST_OFFSET          (CST_OFFSET + 1)
#define TIME_BASE_YEAR		2021

#define SUNNY               0
#define RAINY               1
#define SNOWY               2
#define WINDY               3
#define CLOUDY              4

#define LATITUDE        41.12345
#define LONGITUDE       -87.98765

const uint8_t _usDSTStart[22] = { 14,13,12,10, 9, 8,14,12,11,10, 9,14,13,12,11, 9};
const uint8_t _usDSTEnd[22]   = { 7, 6, 5, 3, 2, 1, 7, 5, 4, 3, 2, 7, 6, 5, 4, 2};
String g_name = "iclock-";
String g_mqttName = g_name + System.deviceID().substring(0, 8);
char mqttServer[] = "172.24.1.13";
int g_timeZone;
int g_weather;
int g_brightness;
int g_ledBright;
String g_conditions;

SunSet sun;
Adafruit_7segment mainclock = Adafruit_7segment();
Adafruit_8x16matrix left = Adafruit_8x16matrix();
Adafruit_8x16matrix right = Adafruit_8x16matrix();
Adafruit_AlphaNum4 temperature = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 humidity = Adafruit_AlphaNum4();

MQTT client(mqttServer, 1883, callback);
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

//{"environment":{"celsius":17.44,"farenheit":63.42799,"humidity":33.2965,"dewpointc":0.996169,"dewpointf":33.79311,"hourlyrain":0,"raintoday":0,"raintotal":22},"appid":109,"time":1616428681}
void callback(char* leftic, byte* payload, unsigned int length) 
{
    int temp = 0;
    int humid = 0;
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = '\0';

    JSONValue outerObj = JSONValue::parseCopy(p);
    JSONObjectIterator iter(outerObj);
    while (iter.next()) {
        if (iter.value().isObject() && iter.name() == "environment") {
            JSONObjectIterator env(iter.value());
            while (env.next()) {
                if (env.name() == "farenheit") {
                    temp = static_cast<int>((env.value().toDouble() + .05) * 10);
                }
                if (env.name() == "humidity") {
                    humid = static_cast<int>((env.value().toDouble() + .05) * 10);
                }
            }
        }
    }
    if (temp >= 1000) {
        temperature.writeDigitAscii(0, 49);
        temp -= 1000;
    }
    uint8_t thigh = temp / 100;
    uint8_t tlow = (temp % 100) / 10;
    uint8_t tdec = temp % 10;
    Log.printf("Parsed temp: %d%d.%d\n", thigh, tlow, tdec);

    temperature.writeDigitAscii(1, 48 + thigh);
    temperature.writeDigitAscii(2, 48 + tlow, true);
    temperature.writeDigitAscii(3, 48 + tdec);
    temperature.writeDisplay();
    if (humid >= 1000) {
        humidity.writeDigitAscii(0, 49);
        humid -= 1000;
    }
    uint8_t hhigh = humid / 100;
    uint8_t hlow = (humid % 100) / 10;
    uint8_t hdec = humid % 10;
    Log.printf("Parsed humidity: %d%d.%d\n", hhigh, hlow, hdec);

    humidity.writeDigitAscii(1, 48 + hhigh);
    humidity.writeDigitAscii(2, 48 + hlow, true);
    humidity.writeDigitAscii(3, 48 + hdec);
    humidity.writeDisplay();
}

void connect_mqtt()
{
    client.connect(g_mqttName);

    // publish/subscribe
    if (client.isConnected()) {
        client.subscribe("weather/conditions");
    }
}

void updateClock()
{
    int currenttime = 0;
    
    currenttime = Time.hour() * 100;
    if (currenttime > 1200)
        currenttime -= 1200;

    currenttime += Time.minute();
    mainclock.print(currenttime, DEC);
    mainclock.drawColon(true);
    mainclock.writeDisplay();
}

void clearDisplays()
{
    Serial.begin(115200);
    
    mainclock.clear();
    temperature.clear();
    humidity.clear();

    mainclock.writeDisplay();
    temperature.writeDisplay();
    humidity.writeDisplay();

}

#if 0
void raindrops()
{
    static int leftx = random(1, 7);
    static int lefty = 2;
    static int rightx = random(1, 7);
    static int righty = 2;
    static system_tick_t last = millis() - 100;

    if (millis() >= last + 100) {
        left.clear();
        right.clear();
        left.drawLine(0, 0, 7, 0, LED_ON);
        left.drawLine(1, 1, 7, 1, LED_ON);
        left.drawLine(2, 2, 6, 2, LED_ON);
        right.drawLine(0, 0, 7, 0, LED_ON);
        right.drawLine(1, 1, 7, 1, LED_ON);
        right.drawLine(2, 2, 6, 2, LED_ON);

        left.drawCircle(leftx, lefty, 1, LED_ON);
        right.drawCircle(rightx, righty, 1, LED_ON);
        if (++lefty >= 15) {
            lefty = 1;
            while (1) {
                int x = random(1, 7);
                if (x != leftx) {
                    leftx = x;
                    break;
                }
            }
        }
        if (++righty >= 15) {
            righty = 1;
            while (1) {
                int x = random(1, 7);
                if (x != rightx) {
                    rightx = x;
                    break;
                }
            }
        }
        left.writeDisplay();
        right.writeDisplay();
        last = millis();
    }
}

void sunImage(int yoffset)
{
    left.fillCircle(7, 7 + yoffset, 5, LED_ON);
    right.fillCircle(0, 7 + yoffset, 5, LED_ON);
    left.drawLine(1, 1 + yoffset, 2, 2 + yoffset, LED_ON);
    right.drawLine(5, 2 + yoffset, 6, 1 + yoffset, LED_ON);
    left.drawPixel(0, 7 + yoffset, LED_ON);
    left.drawPixel(7, 0 + yoffset, LED_ON);
    right.drawPixel(0, 0 + yoffset, LED_ON);
    right.drawPixel(7, 7 + yoffset, LED_ON);
    left.drawPixel(7, 14 + yoffset, LED_ON);
    right.drawPixel(0, 14 + yoffset, LED_ON);
    left.drawLine(1, 13 + yoffset, 2, 12 + yoffset, LED_ON);
    right.drawLine(5, 12 + yoffset, 6, 13 + yoffset, LED_ON);
    left.writeDisplay();
    right.writeDisplay();
}

void cloudImage()
{
    int xoffset = 3;
    int yoffset = 3;
    left.clear();
    right.clear();
    left.fillCircle(0 + xoffset, 0 + yoffset, 3, LED_ON);
    left.fillCircle(5 + xoffset, 3 + xoffset, 3, LED_ON);
    right.fillCircle(3 + xoffset, 1 + yoffset, 3, LED_ON);
    right.fillCircle(0, 6, 3, LED_ON);
    left.writeDisplay();
    right.writeDisplay();
}

#endif

void setBrightness(int bright)
{
    int clock = bright;
    int temp = bright - 9;

    mainclock.setBrightness(clock);
    mainclock.writeDisplay();
    temperature.setBrightness(temp);
    temperature.writeDisplay();
    humidity.setBrightness(temp);
    humidity.writeDisplay();
}

void setDateTime()
{
    int tzoffset = currentTimeZone();

    Particle.syncTime();
    waitUntil(Particle.syncTimeDone);
    sun.setCurrentDate(Time.year(), Time.month(), Time.day());
    sun.setTZOffset(tzoffset);
    Time.zone(tzoffset);
}

// {"coord":{"lon":-87.9806,"lat":42.0884},"weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],"base":"stations","main":{"temp":54.9,"feels_like":41.23,"temp_min":53.01,"temp_max":57,"pressure":1004,"humidity":67},"visibility":10000,"wind":{"speed":21.85,"deg":200,"gust":32.21},"clouds":{"all":90},"dt":1616600803,"sys":{"type":1,"id":5453,"country":"US","sunrise":1616586467,"sunset":1616630896},"timezone":-18000,"id":4883555,"name":"Arlington Heights","cod":200}
void weatherData(const char *event, const char *data) 
{
    JSONValue outerObj = JSONValue::parseCopy(data);
    JSONObjectIterator iter(outerObj);
    while (iter.next()) {
        if (iter.value().isArray() && iter.name() == "weather") {
            JSONArrayIterator array(iter.value());
            for (int i = 0; i < array.next(); i++) {
                if (array.value().isObject()) {
                    JSONObjectIterator conditions(array.value());
                    while (conditions.next()) {
                        if (conditions.name() == "main") {
                            g_conditions = conditions.value().toString().data();
                        }
                    }
                }
            }
        }
    }
    Log.printf("Conditions: %s\n", g_conditions.c_str());
/*
    if (g_conditions == "Clouds") {
        cloudImage();
    }
*/
}

void setup()
{
    connect_mqtt();
    mainclock.begin(0x70);
    temperature.begin(0x71);
    humidity.begin(0x72);
//    left.begin(0x73);
//    right.begin(0x74);

//    Particle.subscribe("hook-response/weather", weatherData);
    Particle.variable("brightness", g_brightness);
    Particle.variable("display", g_ledBright);
    clearDisplays();
    sun.setPosition(LATITUDE, LONGITUDE, currentTimeZone());
}

void loop() 
{
    static int lasthour = 24;
    static int lastSecond = 60;

    g_brightness = analogRead(A0);
    g_ledBright = map(g_brightness, 0, 4098, 10, 15);

    setBrightness(g_ledBright);

    if (Time.hour() != lasthour) {
        setDateTime();
        lasthour = Time.hour();
//        Particle.publish("weather", String(), PRIVATE);
    }

    if (lastSecond != Time.second()) {
        if (client.isConnected()) {
            client.loop();
        }
        else {
            connect_mqtt();
        }
        lastSecond = Time.second();
        updateClock();
    }
}
