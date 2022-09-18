#include <Arduino.h>
#include "Actions.h"
#include <KarakuriMotors.h>

KarakuriMotors motors;

//const int Switch=35;
#define Encoder 32


void Actions::init(){
    pinMode(Switch, INPUT);
    pinMode(Encoder, INPUT);
}

void Actions::movimientos(int comando, int argument)
{
    switch (comando)
    {
    case PAUSA:
        pausa(1);
        delay(argument*1000);
        break;
    case VACIA:
        pausa(5);
        break;
    case ADELANTE:
        adelante(argument);
        pausa(1);
        break;
    case ATRAS:
        atras(argument);
        pausa(1);
        break;
    default:
        pausa(1);
        break;
    }
}
void Actions::pausa(int argument)
{
    for (int k = 0; k < argument; k++)
    {
        motors.setSpeeds(0, 0);
        Serial.print("pausa");
        Serial.print(" ,");
        Serial.println(k);
    }
}

void Actions::adelante(int argument)
{
    for (int k = 0; k < argument; k++)
    {
        motors.setSpeeds(moveSpeed, moveSpeed);
        Serial.print("Ejecutando adelante");
        Serial.print(" ,");
        Serial.println(k);
    }
}

void Actions::atras(int argument)
{
    for (int k = 0; k < argument; k++)
    {
        motors.setSpeeds(-moveSpeed, -moveSpeed);
        Serial.print("Ejecutando atras");
        Serial.print(" ,");
        Serial.println(k);
    }
}

void Actions::tele(int estado)
{
    if (estado == 0)
    {
        motors.setSpeeds(0, 0);
    }
    else if (estado == ADELANTE)
    {
        motors.setSpeeds(moveSpeed, moveSpeed);
    }
    else if (estado == ATRAS)
    {
        motors.setSpeeds(-moveSpeed, -moveSpeed);
    }
    else
    {
        motors.setSpeeds(0, 0);
        delay(2);
    }
}

void Actions::home()
{
      Serial.println(digitalRead(Switch));
      
      if(digitalRead(Switch)!=HIGH){

        motors.setSpeed(0);
         Serial.println("Button Push");
      }

      else{
         Serial.println("Button No Push - Activate Motor");
         motors.setSpeed(100); // SET TEST SPEED

         while(digitalRead(Switch)==HIGH){}
         
         motors.setSpeed(0);
         Serial.println("Button Push");
        
      }
}
