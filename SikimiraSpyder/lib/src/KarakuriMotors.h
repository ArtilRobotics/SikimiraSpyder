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

#ifndef KarakuriMotors_h
#define KarakuriMotors_h

#include <stdint.h>

class KarakuriMotors
{
public:
    static void flipLeftMotor(bool flip);
    static void flipRightMotor(bool flip);
    static void setLeftSpeed(int16_t speed);
    static void setRightSpeed(int16_t speed);
    /**
     * @brief Set the Speeds object
     * 
     * @param leftSpeed 
     * @param rightSpeed 
     */
    static void setSpeeds(int16_t leftSpeed, int16_t rightSpeed);
    void attenuatedSpeeds(float leftSpeed,int16_t rightSpeed);

private:
    static inline void init()
    {
        static bool initialized = false;

        if(!initialized)
        {
            initialized = true;
            init2();
        }
    }
    static void init2();

    float _rightSpeed;
    float _leftSpeed;
};
#endif