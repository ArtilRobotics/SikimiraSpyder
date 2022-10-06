#include <Arduino.h>
#include <TimeClock.h>
#include <WiFi.h>
#include "time.h"
#include <PermanentValues.h>

PermanentValues SetValues;

void TimeClock::init()
{
  initTime("COT5"); // Set for EC
  // initTime("LINT-14");
  printLocalTime();
}

void TimeClock::setTimezone(String timezone)
{
  // Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ", timezone.c_str(), 1); //  Now adjust the COT5.  Clock settings are adjusted to show the new local time
  tzset();
}

void TimeClock::initTime(String timezone)
{
  struct tm timeinfo;

  // Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org"); // First connect to NTP server, with 0 TZ offset
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return;
  }
  // Serial.println("  Got the time from NTP");
  //  Now we can set the real timezone
  setTimezone(timezone);
}

void TimeClock::printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time 1");
    // return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  Serial.println(&timeinfo, "%H:%M:%S");
}

boolean array_cmp(int *a, int *b, int len_a, int len_b)
{
  int n;

  // if their lengths are different, return false
  if (len_a != len_b)
    return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++)
    if (a[n] != b[n])
      return false;

  // ok, if we have not returned yet, they are equal :)
  return true;
}

// bool TimeClock::Compare_Time(){
//   struct tm timeinfo;
//   bool Status=false;
//   if(!getLocalTime(&timeinfo)){}

//    datoCC[0] = timeinfo.tm_hour;
//    datoCC[1] = timeinfo.tm_min;

//    if(datoCC[0]>datoC[0] || datoCC[0]==datoC[0] && datoCC[1]>=datoC[1]){
//       if(datoCC[0]>datoC[0]){
//         Serial.print(datoCC[0]);
//         Serial.print(" > ");
//         Serial.println(datoC[0]);
//       }

//       if(datoCC[0]==datoC[0] && datoCC[1]>=datoC[1]){
//         Serial.print(datoCC[0]);
//         Serial.print(" == ");
//         Serial.print(datoC[0]);
//         Serial.print(" && ");
//         Serial.print(datoCC[1]);
//         Serial.print(" >= ");
//         Serial.println(datoC[1]);
//       }

//     Status = true;
//    }

//    //Serial.print(datoCC[0]);
//    //Serial.print(":");
//    //Serial.println(datoCC[1]);
//    //Serial.println(Status);

//   return Status;
// }

bool TimeClock::Compare_Time()
{
  struct tm timeinfo;
  bool Status = false;

  if (!getLocalTime(&timeinfo))
  {
  }

  datoCC[0] = timeinfo.tm_hour;
  datoCC[1] = timeinfo.tm_min;

  if (datoCC[0] > datoTON[0] && datoCC[0] < datoTOFF[0])
    Status = true;

  if (datoCC[0] >= datoTON[0] && datoCC[0] <= datoTOFF[0])
  {
    if (datoCC[0] == datoTON[0] && datoCC[0] == datoTOFF[0])
    {
      if (datoCC[1] >= datoTON[1] && datoCC[1] < datoTOFF[1])
      {
        Status = true;
      }
    }
    if (datoCC[0] > datoTON[0] && datoCC[0] == datoTOFF[0])
    {
      if (datoCC[1] < datoTOFF[1])
      {
        Status = true;
      }
    }
    if (datoCC[0] == datoTON[0] && datoCC[0] < datoTOFF[0])
    {
      if (datoCC[1] >= datoTON[1])
      {
        Status = true;
      }
    }
  }

  if (datoCC[0] == 0 && datoCC[1]==0 ){ESP.restart();}
  

  return Status;
  // Serial.print(datoCC[0]);
  // Serial.print(":");
  // Serial.println(datoCC[1]);
  // Serial.println(Status);
}

void TimeClock::setTimeON(int datoC[2])
{

  datoTON[0] = datoC[0];
  datoTON[1] = datoC[1];

  Serial.print("Set ON time to: ");
  Serial.print(datoTON[0]);
  Serial.print(":");
  Serial.println(datoTON[1]);

  SetValues.WR_time(true, char(datoTON[0]), char(datoTON[1]), true);
}

void TimeClock::setTimeOFF(int datoC[2])
{

  datoTOFF[0] = datoC[0];
  datoTOFF[1] = datoC[1];

  Serial.print("Set OFF time to: ");
  Serial.print(datoTOFF[0]);
  Serial.print(":");
  Serial.println(datoTOFF[1]);

  SetValues.WR_time(false, char(datoTOFF[0]), char(datoTOFF[1]), true);
}

int TimeClock::TodayDay()
{

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
  }

  int Day = timeinfo.tm_wday;
  return Day;
}
