#include <Arduino.h>
#include "Actions.h"
#include <KarakuriMotors.h>
#include <TimeClock.h>
#include <PermanentValues.h>

KarakuriMotors motors;
TimeClock WifiTime;
PermanentValues SetValues1; 
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
    home();
    
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
    case ARRIBA:
        atrasMin();
        pausa(1);
        break;

    case ABAJO:
        adelanteMax();
        pausa(1);
        break;
    
    case SPIDERONE:
        adelante(100,250);
        pausa(argument);
        atras(90,100);
         pausa(argument);
        atras(80,100);
        pausa(argument);
        atras(70,100);
         pausa(argument);
        atras(60,100);
         pausa(argument);
        atras(50,100);
         pausa(argument);
        adelante(80,250);
         pausa(argument);
        atras(20,100);
         pausa(argument);
        atras(0,100);

        break;
    case SPIDERTWO:
        
        pausa(1);
        break;
    default:
        pausa(1);
        break;
    }
}
void Actions::pausa(int argument)
{

    Serial.println("Se detiene el motor");
     motors.setSpeed(0);
    Serial.print("Espera por ");
    Serial.print(argument);
    Serial.println("s");

    delay(argument*1000);
    
}

void Actions::adelante(int Porcentage, int Speed)
{
    getLevels();

    float position = SetValues1.ValueLow * (float(Porcentage)/100.);

    Serial.print("Ejecutando adelante hasta llegar a la posición máxima: ");
    Serial.println(position);
    motors.directionM=true;
    motors.setSpeed(Speed);

    while(motors.lenght<position){
        MovingLoop();
    } 
    Serial.println("Llego a la posición Final Definida");
    motors.setSpeed(0);


}

void Actions::atras(int Porcentage, int Speed)
{
    getLevels();

    float position = SetValues1.ValueLow * (float(Porcentage)/100);
    
    if(position<SetValues1.ValueHigh){

        Serial.print("Ejecutando adelante hasta llegar a la posición: ");
        Serial.println(SetValues1.ValueHigh);
        motors.directionM=false;
        motors.setSpeed(-Speed);

        while(motors.lenght>SetValues1.ValueHigh){
            MovingLoop();
        } 
        Serial.println("Llego a la posición Final Definida");
        motors.setSpeed(0);

    }

    if(position>=SetValues1.ValueHigh){

        Serial.print("Ejecutando atras hasta llegar a la posición: ");
        Serial.println(position);
        motors.directionM=false;
        motors.setSpeed(-Speed);

        while(motors.lenght>position){
            MovingLoop();
        } 
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
    motors.directionM=true;
    motors.setSpeed(maxSpeed);

    while(motors.lenght<SetValues1.ValueHigh){
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
     //digitalWrite(ledPin, SpiderTimeState);
 //}
        

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

void Actions::CheckDayStatus(){

   SetValues1.WR_day(WifiTime.TodayDay(), 0, false);
   SpiderDayState = SetValues1.dayStatus;
}


void Actions::CheckTimer(){ // Funcion para verificar el horario de activacion
    getTimesProgram();
    //Falta agregregar la funcion para comprobar el dia y que este active o no  la varibale DeviceTimeStatus
    //Agregar como funcion de verificacion dentro de la libreria TimeClock 

    if (DeviceTimeStatus == false){
         WifiTime.datoTC[0] = SetValues1.ValueHourON;
         WifiTime.datoTC[1] = SetValues1.ValueMinON;
        ///////////////////////////////////////////
         Serial.println("DeviceTimeStatus = false");
         Serial.print("TimeON:");
         Serial.print(WifiTime.datoTC[0]);
         Serial.print(":");
         Serial.println(WifiTime.datoTC[1]);
        /////////////////////////////////////////////
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            Serial.println("Actvivated");
            DeviceTimeStatus=true;
            home();
          };
      }
    
      else if (DeviceTimeStatus == true){
         WifiTime.datoTC[0] = SetValues1.ValueHourOFF;
         WifiTime.datoTC[1] = SetValues1.ValueMinOFF;
          ///////////////////////////////////////////
         Serial.println("DeviceTimeStatus = true");
         Serial.print("TimeOFF:");
         Serial.print(WifiTime.datoTC[0]);
         Serial.print(":");
         Serial.println(WifiTime.datoTC[1]);
        /////////////////////////////////////////////
          if (WifiTime.Compare_Time(WifiTime.datoTC)==true){
            Serial.println("Desactvivated");
            DeviceTimeStatus=false;
            home();
          };
      }

      digitalWrite(ledPin, DeviceTimeStatus);

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






