#include <Arduino.h>

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

DS1307RTC ds1307 = DS1307RTC();
tmElements_t tm;

bool setInitRTC(DS1307RTC *_rtc, tmElements_t &_time);
bool getTime(const char *str);
bool getDate(const char *str);
void print2digits(int number);
bool printTime(DS1307RTC *_rtc, tmElements_t &_time);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (setInitRTC(&ds1307, tm))
  {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  }
  else
  {
    Serial.println("DS1307 Communication Error :-{");
    Serial.println("Please check your circuitry");
  }
}

void loop()
{
  if (printTime(&ds1307, tm))
  {
    ;
  }
  else
  {
    Serial.println("DS1307 read error!  Please check the circuitry.");
    Serial.println();
  }

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

bool printTime(DS1307RTC *_rtc, tmElements_t &_time)
{
  if (_rtc->read(_time))
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

    return true;
  }
  else
  {
    return false;
  }
}