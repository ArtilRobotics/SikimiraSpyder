#include <Arduino.h>
#include "Actions.h"
#include <KarakuriMotors.h>
#include <TimeClock.h>
#include <PermanentValues.h>

KarakuriMotors motors;
TimeClock WifiTime;
PermanentValues SetValues1;

int ledPin = 2;
const int Switch = 35;
long max_lenght = 2000;
int maxSpeed = 250;

bool DeviceTimeStatus = false; // Variable para activar o no el movimiento automático

long previousMillis = 0;
long intervalScan = 50;

long previousMillis2 = 0;
long intervalScan2 = 500;
int Sequence_Now;

void Actions::init()
{
    WifiTime.init();
    pinMode(Switch, INPUT);
    // pinMode(Encoder, INPUT);
    pinMode(ledPin, OUTPUT);
    motors.init3();
    getTimesProgram();
    getLevels();
    CheckDayStatus();
    CheckTimer();
    home();

    // motors
}

void Actions::movimientos(int comando, int argument)
{
    int random_num;

    switch (comando)
    {
    case PAUSA:
        pausa(1);
        delay(argument * 1000);
        break;
    case ARRIBA:
        atrasMin();
        pausa(1);
        break;

    case ABAJO:
        adelanteMax();
        pausa(1);
        break;

    case SPIDERONE:
        Serial.println("SPIDERONE");
        adelante(100, 250);
        pausa(argument);
        atras(0, 100);
        pausa(argument);
        atras(100, 100);
        pausa(argument);
        atras(50, 100);
        pausa(argument);
        atras(100, 100);
        pausa(argument);
        atras(0, 100);
        pausa(1000);

        break;
    case SPIDERTWO:
        Serial.println("SPIDERTWO");
        adelante(20, 250);
        pausa(argument);
        adelante(40, 250);
        pausa(argument);
        adelante(60, 250);
        pausa(argument);
        adelante(80, 250);
        pausa(argument);
        adelante(100, 250);
        pausa(argument);
        atras(0, 100);
        pausa(1000);
        break;
    
    case SPIDERTHREE:
        Serial.println("SPIDERTHREE");
        adelante(100, 250);
        pausa(argument);
        atras(90, 100);
        pausa(argument);
        atras(80, 100);
        pausa(argument);
        atras(70, 100);
        pausa(argument);
        atras(60, 100);
        pausa(argument);
        atras(50, 100);
        pausa(argument);
        adelante(80, 250);
        pausa(argument);
        atras(20, 100);
        pausa(argument);
        atras(0, 100);
        pausa(1000);
        break;

    case RANDOM:
        Serial.println("RANDOM");
        random_num = 1 + (rand() % 3);
        movimientos(random_num, 500);
    break;

    default:
        pausa(100);
        break;
    }
}
void Actions::pausa(int argument)
{

    Serial.println("Se detiene el motor");
    motors.setSpeed(0);
    Serial.print("Espera por ");
    Serial.print(argument);
    Serial.println("ms");

    delay(argument );
}

void Actions::adelante(int Porcentage, int Speed)
{   

    getLevels();

    float position = SetValues1.ValueHigh +(SetValues1.ValueLow - SetValues1.ValueHigh ) * (float(Porcentage) / float (100));

    Serial.print("Ejecutando adelante hasta llegar a la posición máxima: ");
    Serial.println(position);
    motors.directionM = true;
    motors.setSpeed(Speed);

    while (motors.lenght < position)
    {
        MovingLoop();
    }

    if((motors.lenght >= position)) delay(100);
    Serial.println("Llego a la posición Final Definida");
    motors.setSpeed(0);
}

void Actions::atras(int Porcentage, int Speed)
{
    getLevels();

    float position = SetValues1.ValueHigh +(SetValues1.ValueLow - SetValues1.ValueHigh ) * (float(Porcentage) / float (100));

    if (position >= SetValues1.ValueHigh)
    {

        Serial.print("Ejecutando atras hasta llegar a la posición: ");
        Serial.println(position);
        motors.directionM = false;
        motors.setSpeed(-Speed);

        while (motors.lenght > position)
        {
            MovingLoop();
        }
        if((motors.lenght <= position)) delay(100);

        Serial.println("Llego a la posición Final Definida");
        motors.setSpeed(0);
    }
}

void Actions::adelanteMax()
{
    // for (int k = 0; k < argument; k++)
    getLevels();
    Serial.print("Ejecutando adelante hasta llegar a la posición máxima: ");
    Serial.println(SetValues1.ValueHigh);
    motors.directionM = true;
    motors.setSpeed(maxSpeed);

    while (motors.lenght < SetValues1.ValueHigh)
    {
        MovingLoop();
    }
    Serial.println("Llego a la posición Final Máxima");
    motors.setSpeed(0);
}

void Actions::atrasMin()
{
    getLevels();
    Serial.print("Ejecutando atras hasta llegar a la posición minima: ");
    Serial.println(SetValues1.ValueLow);
    motors.directionM = false;
    motors.setSpeed(maxSpeed);

    while (motors.lenght > SetValues1.ValueLow)
    {
        MovingLoop();
    }
    Serial.println("Llego a la posición Final Minima");
    motors.setSpeed(0);
}

void Actions::tele(int estado)
{
}

void Actions::home()
{
    Serial.println(digitalRead(Switch));

    if (digitalRead(Switch) != HIGH)
    {

        motors.setSpeed(0);
        Serial.println("Button Push");
    }

    else
    {
        Serial.println("Button No Push - Activate Motor");
        motors.setSpeed(-100); // SET TEST SPEED

        while (digitalRead(Switch) == HIGH)
        {
        }

        motors.setSpeed(0);
        Serial.println("Button Push");
    }

    motors.setSpeed(10);
    delay(1000);
    motors.setSpeed(0);
    motors.lenght = 0;
    delay(500);
    adelante(0,10);
}

void Actions::CheckLenght()
{
    if ((motors.lenght <= 0 && motors.directionM == false) || digitalRead(Switch) == LOW)
    {
        motors.setSpeed(0);
        motors.lenght = 0;

            home();
    }

    else if (motors.lenght > max_lenght && motors.directionM)
    {
        motors.setSpeed(0);
    }
}

void Actions::CalcLenght()
{
    motors.movingSpyder(motors.directionM);
}

void Actions::MotorStatus()
{

    Serial.print(" Status Motor - La Posicion es: ");
    Serial.println(motors.lenght);
}

//  void Actions::TimerCheck(){

//     if (SpiderTimeState == false){
//          WifiTime.datoTC[0] = WifiTime.datoTON[0];
//          WifiTime.datoTC[1] = WifiTime.datoTON[1];

//           if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
//             SpiderTimeState=true;
//             Serial.println("Actvivated");
//           };
//       }

//       else if ((SpiderTimeState == true)){
//          WifiTime.datoTC[0] = WifiTime.datoTOFF[0];
//          WifiTime.datoTC[1] = WifiTime.datoTOFF[1];
//          ///////////////////////////////////////////
//          Serial.println("SpiderTimeState = true");
//          Serial.print("TimeOFF:");
//          Serial.print(WifiTime.datoTC[0]);
//          Serial.print(":");
//          Serial.println(WifiTime.datoTC[1]);
//         /////////////////////////////////////////////
//           if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
//             SpiderTimeState=false;
//             Serial.println("Desactvivated");
//           };

//       }

// Hacemos que el contenido de la variable llegue al LED
// digitalWrite(ledPin, SpiderTimeState);
//}

void Actions::MoveSpyder(int dato)
{
    Serial.print("Dato Recibido: ");
    Serial.println(dato);
    
    
    float position = SetValues1.ValueHigh +(SetValues1.ValueLow - SetValues1.ValueHigh ) * ((float) dato / (float) 100);
    Serial.print("Dato Traducido (Position): ");
    Serial.println(position);

    if (position > motors.lenght)
    {
        adelante(dato, 25);
    }
    if (position < motors.lenght)
    {
        atras(dato, 25);
    }

    // motors.setSpeed(dato);
}

void Actions::TimeSync()
{
    WifiTime.initTime();
}

void Actions::GetTimeNow()
{
    WifiTime.printLocalTime();
}

void Actions::setON(int datoR[3])
{

    int datoS[2];

    datoS[0] = datoR[1];
    datoS[1] = datoR[2];

    WifiTime.setTimeON(datoS);
}

void Actions::setOFF(int datoR[3])
{

    int datoS[2];

    datoS[0] = datoR[1];
    datoS[1] = datoR[2];

    WifiTime.setTimeOFF(datoS);
}

void Actions::getTimesProgram()
{
    //    SetValues1.WR_time(true, 0,0,false);
    //    SetValues1.WR_time(false, 0,0,false);

    SetValues1.getTime();

    WifiTime.datoTON[0] = SetValues1.ValueHourON;
    WifiTime.datoTON[1] = SetValues1.ValueMinON;
    WifiTime.datoTOFF[0] = SetValues1.ValueHourOFF;
    WifiTime.datoTOFF[1] = SetValues1.ValueMinOFF;
}

void Actions::PrintTimes()
{
    getTimesProgram();
    GetTimeNow();
    Serial.print("Hora de activación: ");
    Serial.print(WifiTime.datoTON[0]);
    Serial.print(":");
    Serial.println(WifiTime.datoTON[1]);

    Serial.print("Hora de desactivación: ");
    Serial.print(WifiTime.datoTOFF[0]);
    Serial.print(":");
    Serial.println(WifiTime.datoTOFF[1]);

    Serial.print("Dias de activación: ");
    Serial.print("L");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[0]);
    Serial.print("-");
    Serial.print("M");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[1]);
    Serial.print("-");
    Serial.print("W");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[2]);
    Serial.print("-");
    Serial.print("J");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[3]);
    Serial.print("-");
    Serial.print("V");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[4]);
    Serial.print("-");
    Serial.print("S");
    Serial.print(":");
    Serial.print(SetValues1.dayConfig[5]);
    Serial.print("-");
    Serial.print("D");
    Serial.print(":");
    Serial.println(SetValues1.dayConfig[6]);

    Serial.print("Secuencia Activada: ");
    Serial.println(SetValues1.Sequence_Select);
}

void Actions::setHigh(int datoR)
{

    SetValues1.WR_distance(true, datoR, true);
}

void Actions::setLow(int datoR)
{

    SetValues1.WR_distance(false, datoR, true);
}

void Actions::getLevels()
{
    SetValues1.WR_distance(true, 0, false);
    SetValues1.WR_distance(false, 0, false);
}

void Actions::PrintLevels()
{
    Serial.print("Nivel Alto: ");
    Serial.println(SetValues1.ValueHigh);

    Serial.print("Nivel Bajo: ");
    Serial.println(SetValues1.ValueLow);

    Serial.print("Longitud Actual: ");
    Serial.println(motors.lenght);
}

void Actions::setDays(int datoR[3])
{
    bool SetValue;

    if (datoR[2] == 1)
    {
        SetValue = true;
    }
    else if (datoR[2] == 0)
    {
        SetValue = false;
    }

    SetValues1.WR_day(datoR[1], SetValue, true);
}

void Actions::CheckDayStatus()
{
    //Serial.print("Today is: ");
    //Serial.println(WifiTime.TodayDay());
    SetValues1.WR_day(WifiTime.TodayDay(), 0, false);
    SpiderDayState = SetValues1.dayStatus;
    //if(SpiderDayState==true){Serial.println("Today Spider Activate");}
    
}

void Actions::CheckTimer()
{ // Funcion para verificar el horario de activacion
    getTimesProgram();
    // Falta agregregar la funcion para comprobar el dia y que este active o no  la varibale DeviceTimeStatus
    // Agregar como funcion de verificacion dentro de la libreria TimeClock

    if (DeviceTimeStatus == false)
    {

        ///////////////////////////////////////////
        Serial.println("DeviceTimeStatus = false");
        Serial.print("TimeON:");
        Serial.print(WifiTime.datoTON[0]);
        Serial.print(":");
        Serial.println(WifiTime.datoTON[1]);
        /////////////////////////////////////////////
        if (WifiTime.Compare_Time() == true)
        {
            Serial.println("Actvivated");
            DeviceTimeStatus = true;
            // home();
        };
    }

    else if (DeviceTimeStatus == true)
    {

        ///////////////////////////////////////////
        Serial.println("DeviceTimeStatus = true");
        Serial.print("TimeOFF:");
        Serial.print(WifiTime.datoTOFF[0]);
        Serial.print(":");
        Serial.println(WifiTime.datoTOFF[1]);
        /////////////////////////////////////////////
        if (WifiTime.Compare_Time() == false)
        {
            Serial.println("Desactvivated");
            DeviceTimeStatus = false;
            home();
        };
    }

    digitalWrite(ledPin, DeviceTimeStatus);
}

void Actions::SetSequence(int Sequence){
        SetValues1.WR_Sequence(Sequence, 0, true);
    }

void Actions::SetPeriodSequence(int Period){
        SetValues1.WR_Sequence(0, Period, true);
    }



void Actions::MovingLoop()
{

        unsigned long currentMillis = millis(); // Se toma el tiempo actual

        if (currentMillis - previousMillis > intervalScan)
        {

            previousMillis = currentMillis;

            // act.GetTimeNow();
            CalcLenght();
        }

        unsigned long currentMillis2 = millis(); // Se toma el tiempo actual

        if (currentMillis2 - previousMillis2 > intervalScan2)
        {

            previousMillis2 = currentMillis2;

            // act.GetTimeNow();
            MotorStatus();
        }

        CheckLenght();

}

int Actions::Sequence_Update(){
    SetValues1.WR_Sequence(0,0,false);
    Period=SetValues1.PeriodSequence;
    Serial.print("Periodo de Secuencia: ");
    Serial.println(Period);
    return SetValues1.Sequence_Select;
}