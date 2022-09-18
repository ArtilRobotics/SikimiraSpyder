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
    void setSpeed(int16_t speed);
    /**
     * @brief Set the Speeds object
     * 
     * @param leftSpeed 
     * @param rightSpeed 
     */
    static void setSpeeds(int16_t leftSpeed, int16_t rightSpeed);
    void attenuatedSpeeds(float leftSpeed,int16_t rightSpeed);

    float velocitysmoothed_R[2];


    void moveSpyder(int intervalTime_input, bool directtionBool_input);
    void speedSpyder(int intervalTime_input);
    static void interrupcion();
    int sensor= 19;
    long lenght;
    bool directionM=false;
    int n_holes=20;
    float pi=3.1416;
    float radius=2.54;
    float revs = 0;
    
    
    //float lenght;

private:
     inline void init()
    {
         bool initialized = false;

        if(!initialized)
        {
            initialized = true;
            init2();
        }
    }
    void init2();

    float _rightSpeed;
    float _leftSpeed;
    
};
#endif