#include <Arduino.h>
#include <KarakuriMotors.h>
#include <KarakuriBluetooth.h>
#include <Actions.h>
#include <WifiManagement.h>

Actions Acciones;
KarakuriBluetooth BTS;
WifiManagement WifiBT;

String strT = "";
const char separatorT = ',';
const int dataLengthT = 3;
int datoT[dataLengthT];

//////////////////////////////////////////////////////
void setup()
{
  // put your setup code here, to run once:

  Serial.begin(115200);
  BTS.Start();
  WifiBT.init();
  Acciones.init();
  

    //  WifiTime.datoTON[0] = SetValues1.ValueHourON;
    //  WifiTime.datoTON[1] = SetValues1.ValueMinON;

    //  WifiTime.datoTOFF[0] = SetValues1.ValueHourOFF;
    //  WifiTime.datoTOFF[1] =  SetValues1.ValueMinOFF;

}

////////////////////////////////////////////////////////////////

void loop()
{


  // put your main code here, to run repeatedly:
  //
  strT = "";
  if (Serial.available())
  {
    strT = Serial.readStringUntil('\n');
    Serial.println(strT);
    for (int i = 0; i < dataLengthT; i++)
    {
      int index = strT.indexOf(separatorT);
      datoT[i] = strT.substring(0, index).toInt();
      strT = strT.substring(index + 1);
    }
    for (int i = 0; i < dataLengthT; i++)
    {
      Serial.printf("Dato %d = %d  ", i, datoT[i]);
    }
    Serial.println(" ");
  }
  switch ((int)datoT[0])
  {
  case 0:
    
    datoT[0] = 0;
    break;
  case 1:
    
    //motors1.setSpeed(0);
    datoT[0] = 0;
    break;

  case 2:
    
    //Serial.println(datoT[1]);
    Acciones.MoveSpyder(datoT[1]);


    datoT[0] = 0;
    break;

    case 3:
    Acciones.home();
    datoT[0] = 0;

    break;


    case 4:
    
    datoT[0] = 0;

    break;

    case 5:
    //WifiTime.setTimeON(datoT);
    datoT[0] = 0;

    break;

    case 6:
    //WifiTime.setTimeOFF(datoT);
    datoT[0] = 0;
    break;

    case 7:
    
    //ledState=HIGH;
    //digitalWrite(ledPin, ledState);
    datoT[0] = 0;
    break;

    case 8:
    //ledState=LOW;
    //digitalWrite(ledPin, ledState);
     datoT[0] = 0;
    break;
    
    case 9:
    
    Acciones.movimientos(20,2);
     //datoT[0] = 0;
    break;

    case 10:
   // Serial.print("Longitud motors1.directionM: ");
    //Serial.println(motors1.directionM);

        Acciones.getTimesProgram();
        Acciones.getLevels();
        Acciones.PrintTimes();
        Acciones.PrintLevels();
        

     datoT[0] = 0;
    break;

    default:
    // colocar movimientos pero en cada instante
    Serial.println("Sistema tele operado");
    // act.tele((int)dato[1]);
    datoT[0] = 0;

  }

  BTS.Update();

}