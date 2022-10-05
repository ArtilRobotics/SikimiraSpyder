#pragma once

#include <Arduino.h>

#include <KarakuriMotors.h>

extern KarakuriMotors motors;

class Actions
{
    const int moveSpeed = 150;
    const int minSpeed = 48;
    const int factor = 23;
    //const int Switch;

    enum Commands
    {
        PAUSA = 80,
        SPIDERONE = 20,
        SPIDERTWO = 10,
        ARRIBA = 70,
        ABAJO = 66
    };

public:
    void init();
    void movimientos(int comando, int argument);
    void pausa(int argument);
    void adelante(int Porcentage, int Speed);
    void atras(int Porcentage, int Speed);
    void tele(int estado);

    void adelanteMax();
    void atrasMin();

    void home();
    void CheckLenght();   
    void CalcLenght();
    void MotorStatus();
    void TimerCheck();

    void MoveSpyder(int dato);

    void GetTimeNow();

    const int Switch=35;

    void setON(int datoR[3]);
    void setOFF(int datoR[3]);
    void setHigh(int dato);
    void setLow(int dato);
    void getTimesProgram();
    void PrintTimes();
    void getLevels();
    void PrintLevels();
    void setDays(int datoR[3]);

    void CheckDayStatus();
    void CheckTimer();

    void MovingLoop();

    bool SpiderTimeState=false;
    bool SpiderDayState=false;
    bool DeviceTimeStatus;
private:
};