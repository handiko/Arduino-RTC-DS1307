#include <Arduino.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
#define OLED_RST 4

const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

DS1307RTC ds1307 = DS1307RTC();
tmElements_t tm;

Adafruit_SSD1306 oled = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

bool setInitRTC(DS1307RTC *_rtc, tmElements_t &_time);
bool getTime(const char *str);
bool getDate(const char *str);
void print2digits(int number);
void printTime(tmElements_t &_time);

bool initOLED(Adafruit_SSD1306 *_oled, uint16_t _oled_address);

void setup()
{
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println(F(" "));

  initOLED(&oled, OLED_ADDRESS);

  if (ds1307.read(tm))
  {
    printTime(tm);
  }
  else
  {
    Serial.println("DS1307 read error!  Please check the circuitry.");
    Serial.println();
  }
}

void loop()
{
  if (ds1307.read(tm))
  {
    printTime(tm);
  }
  else
  {
    Serial.println("DS1307 read error!  Please check the circuitry.");
    Serial.println();
  }

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.println(tm.Second);
  oled.display();

  delay(900);
}

bool setInitRTC(DS1307RTC *_rtc, tmElements_t &_time)
{
  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__))
  {
    // and configure the RTC with this info
    if (_rtc->write(_time))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
    return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3)
    return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++)
  {
    if (strcmp(Month, monthName[monthIndex]) == 0)
      break;
  }
  if (monthIndex >= 12)
    return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
}

void print2digits(int number)
{
  if (number >= 0 && number < 10)
  {
    Serial.write('0');
  }
  Serial.print(number);
}

void printTime(tmElements_t &_time)
{
  Serial.print("Ok, Time = ");
  print2digits(_time.Hour);
  Serial.write(':');
  print2digits(_time.Minute);
  Serial.write(':');
  print2digits(_time.Second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(_time.Day);
  Serial.write('/');
  Serial.print(_time.Month);
  Serial.write('/');
  Serial.print(tmYearToCalendar(_time.Year));
  Serial.println();
}

bool initOLED(Adafruit_SSD1306 *_oled, uint16_t _oled_address)
{
  if (_oled->begin(SSD1306_SWITCHCAPVCC, _oled_address))
  {
    _oled->clearDisplay();
    _oled->setTextColor(SSD1306_WHITE);
    _oled->display();

    return true;
  }
  else
  {
    return false;
  }
}