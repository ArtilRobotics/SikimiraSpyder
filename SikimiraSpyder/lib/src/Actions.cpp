#include <Arduino.h>
#include "Actions.h"
#include <KarakuriMotors.h>
#include <TimeClock.h>
#include <PermanentValues.h>

KarakuriMotors motors;
TimeClock WifiTime;
PermanentValues SetValues1;
int ledState = LOW; 
int ledPin = 2 ;
const int Switch=35;
long max_lenght=2000;
int maxSpeed=50;

bool DeviceTimeStatus=false; //Varibale para activar o no el movimiento automatico

long previousMillis = 0;
long intervalScan = 1000; 

long previousMillis2 = 0;
long intervalScan2 = 100; 


void Actions::init(){
    WifiTime.init();
    pinMode(Switch, INPUT);
    //pinMode(Encoder, INPUT);
    pinMode(ledPin, OUTPUT);
    motors.init3();
    getTimesProgram();
    getLevels();
    
    //motors
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
    // for (int k = 0; k < argument; k++)
    // {
    //     // motors.setSpeeds(0, 0);
    //     // Serial.print("pausa");
    //     // Serial.print(" ,");
    //     // Serial.println(k);
    // }
    Serial.println("Se detiene el motor");
     motors.setSpeed(0);
}

void Actions::adelante(int argument)
{
    // for (int k = 0; k < argument; k++)
    // {
    //     motors.setSpeeds(moveSpeed, moveSpeed);
    //     Serial.print("Ejecutando adelante");
    //     Serial.print(" ,");
    //     Serial.println(k);
    // }
    getLevels();
    Serial.print("Ejecutando adelante hasta llegar a la posición máxima: ");
    Serial.println(SetValues1.ValueHigh);
    motors.directionM=true;
    motors.setSpeed(maxSpeed);

    while(motors.lenght<SetValues1.ValueHigh){
        MovingLoop();
    } 
    Serial.println("Llego a la posición Final Máxima");
    motors.setSpeed(0);


}

void Actions::atras(int argument)
{
    // for (int k = 0; k < argument; k++)
    // {
    //     motors.setSpeeds(-moveSpeed, -moveSpeed);
    //     Serial.print("Ejecutando atras");
    //     Serial.print(" ,");
    //     Serial.println(k);
    // }
    getLevels();
    Serial.print("Ejecutando atras hasta llegar a la posición minima: ");
    Serial.println(SetValues1.ValueLow);
    motors.directionM=false;
    motors.setSpeed(maxSpeed);

    while(motors.lenght>SetValues1.ValueLow){
        MovingLoop();
    } 
    Serial.println("Llego a la posición Final Minima");
    motors.setSpeed(0);
}

void Actions::tele(int estado)
{
    // if (estado == 0)
    // {
    //     motors.setSpeeds(0, 0);
    // }
    // else if (estado == ADELANTE)
    // {
    //     motors.setSpeeds(moveSpeed, moveSpeed);
    // }
    // else if (estado == ATRAS)
    // {
    //     motors.setSpeeds(-moveSpeed, -moveSpeed);
    // }
    // else
    // {
    //     motors.setSpeeds(0, 0);
    //     delay(2);
    // }
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

void Actions::CheckLenght(){
            if((motors.lenght<=0 && motors.directionM==false) || digitalRead(Switch)== LOW){
                motors.setSpeed(0);
                motors.lenght=0;
                
            }

            else if(motors.lenght>max_lenght && motors.directionM){
                motors.setSpeed(0);
            }
    }

   
 void Actions::CalcLenght(){
    motors.movingSpyder(motors.directionM);
 }

 void Actions::MotorStatus(){

      Serial.print(" La Posicion es: ");
      Serial.println(motors.lenght);
 }

 void Actions::TimerCheck(){

    if (ledState == LOW){
         WifiTime.datoTC[0] = WifiTime.datoTON[0];
         WifiTime.datoTC[1] = WifiTime.datoTON[1];
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            ledState=true;
          };
      }
    
      else if ((ledState == HIGH)){
         WifiTime.datoTC[0] = WifiTime.datoTOFF[0];
         WifiTime.datoTC[1] = WifiTime.datoTOFF[1];
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            ledState=false;
          };

      }
         

      // Hacemos que el contenido de la variable llegue al LED
      digitalWrite(ledPin, ledState);
 }
        

void Actions::MoveSpyder(int dato)
{
    
        motors.setSpeed(dato);
       
}

void Actions::GetTimeNow(){
    WifiTime.printLocalTime();
}

void Actions::setON(int datoR[3]){
   
    int datoS[2];

    datoS[0] = datoR[1];
    datoS[1] = datoR[2];

    WifiTime.setTimeON(datoS);
}

void Actions::setOFF(int datoR[3]){
     
    int datoS[2];

    datoS[0] = datoR[1];
    datoS[1] = datoR[2];

    WifiTime.setTimeOFF(datoS);
}

void Actions::getTimesProgram(){
   SetValues1.WR_time(true, 0,0,false);
   SetValues1.WR_time(false, 0,0,false);
}

void Actions::PrintTimes(){
    Serial.print("Hora de activación: ");
    Serial.print(SetValues1.ValueHourON);
    Serial.print(":");
    Serial.println(SetValues1.ValueMinON);

    
    Serial.print("Hora de desactivación: ");
    Serial.print(SetValues1.ValueHourOFF);
    Serial.print(":");
    Serial.println(SetValues1.ValueMinOFF);
}


void Actions::setHigh(int datoR){
   
  SetValues1.WR_distance(true, datoR, true);
}

void Actions::setLow(int datoR){
   
  SetValues1.WR_distance(false, datoR, true);
}


void Actions::getLevels(){
   SetValues1.WR_distance(true, 0,false);
   SetValues1.WR_distance(false, 0,false);
}

void Actions::PrintLevels(){
    Serial.print("Nivel Alto: ");
    Serial.println(SetValues1.ValueHigh);

     Serial.print("Nivel Bajo: ");
    Serial.println(SetValues1.ValueLow);
}

void Actions::setDays(int datoR[3]){
    bool SetValue;
    
    if(datoR[2] == 1){SetValue=true;} 
    else if(datoR[2] == 0){SetValue=false;}


    SetValues1.WR_day(datoR[1], SetValue, true);

}

void Actions::getDayStatus(int Day){

  //Esta funcion permite conocer si esta activado o no en un dia dado
  //El resultado es dado en la variable dayStatus de la libreria Permanent Values
   
   SetValues1.WR_day(Day, 0, false);

}

void Actions::ChechTimer(int Day){ // Funcion para verificar el horario de activacion
    getTimesProgram();
    getDayStatus(Day);
    //Falta agregregar la funcion para comprobar el dia y que este active o no  la varibale DeviceTimeStatus
    //Agregar como funcion de verificacion dentro de la libreria TimeClock 
    
    if (DeviceTimeStatus == false){
         WifiTime.datoTC[0] = SetValues1.ValueHourON;
         WifiTime.datoTC[1] = SetValues1.ValueMinON;
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            DeviceTimeStatus=true;
          };
      }
    
      else if (DeviceTimeStatus == true){
         WifiTime.datoTC[0] = SetValues1.ValueHourOFF;
         WifiTime.datoTC[1] = SetValues1.ValueMinOFF;
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            DeviceTimeStatus=false;
          };

      }

}

void Actions::MovingLoop(){
    {
        
      unsigned long currentMillis = millis();    // Se toma el tiempo actual

 
            if (currentMillis - previousMillis > intervalScan){

                    previousMillis = currentMillis;
            
                    //act.GetTimeNow();
                CalcLenght();
                    
                }

        unsigned long currentMillis2 = millis();    // Se toma el tiempo actual

 
            if (currentMillis2 - previousMillis2 > intervalScan2){

                previousMillis2 = currentMillis2;
        
                //act.GetTimeNow();
                MotorStatus();
            
            }

            CheckLenght();
    } 
         
}





