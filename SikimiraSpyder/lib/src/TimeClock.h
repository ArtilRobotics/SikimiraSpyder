/**
 * @file KarakuriMotors.h
 * @author Marcelo Ortiz (marcelo@artilrobotics.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-18
 * @endcode
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once

#ifndef TimeClock_h
#define TimeClock_h

#include <Wifi.h>

 class TimeClock
 {
 public:
    void init();
    void setTimezone(String timezones);
    void initTime(String timezones);
    void printLocalTime();
    bool Compare_Time( int dato[2]);
    void setTimeON( int dato[2]);
    void setTimeOFF( int dato[2]);

    int datoTC[2];
    int datoCC[2];
    int datoTON[2];
    int datoTOFF[2];

 private:


 };
 #endif