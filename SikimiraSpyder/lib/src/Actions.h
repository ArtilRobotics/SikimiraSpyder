#pragma once

#include <Arduino.h>

#include <KarakuriMotors.h>

extern KarakuriMotors motors;

class Actions
{
    const int moveSpeed = 150;
    const int minSpeed = 48;
    const int factor = 23;

    enum Commands
    {
        VACIA = 0,
        PAUSA = 80,
        ADELANTE = 70,
        ATRAS = 66,
    };

public:
    void movimientos(int comando, int argument);
    void pausa(int argument);
    void adelante(int argument);
    void atras(int argument);
    void tele(int estado);

private:
};