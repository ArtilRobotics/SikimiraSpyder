#include <Arduino.h>
#include <KarakuriMotors.h>
#include <KarakuriBluetooth.h>
#include <Actions.h>
//

#include <WiFi.h>
#include "time.h"

const char * ssid="Casa_PP_T";
const char * wifipw="Casa151098#";

//


Actions Acciones;
KarakuriMotors motors1;
KarakuriBluetooth BTS;

String strT = "";
const char separatorT = ',';
const int dataLengthT = 3;
int datoT[dataLengthT];

int datoTC[2];
int datoCC[2];
int datoTON[2];
int datoTOFF[2];

int ledState = LOW; 
int ledPin = 2 ;
long previousMillis = 0;
  long intervalScan = 1000; 
//////////////////////////////////////////////////


void setTimezone(String timezone){
  //Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone){
  struct tm timeinfo;

  //Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  //Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    //return;
  }
 // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  Serial.println(&timeinfo, "%H:%M:%S");
}

void  startWifi(){
  WiFi.begin(ssid, wifipw);
  Serial.println("Connecting Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Wifi RSSI=");
  //Serial.println(WiFi.RSSI());
}


boolean array_cmp(int *a, int *b, int len_a, int len_b){
      int n;

      // if their lengths are different, return false
      if (len_a != len_b) return false;

      // test each element to be the same. if not, return false
      for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;

      //ok, if we have not returned yet, they are equal :)
      return true;
}

  bool Compare_Time( int datoC[2]){
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){}
  
     datoCC[0] = timeinfo.tm_hour;
     datoCC[1] = timeinfo.tm_min;
    
    bool Status = array_cmp(datoC,datoCC,2,2);
     Serial.print(datoCC[0]);
     Serial.print(":");
     Serial.println(datoCC[1]);
     Serial.println(Status);

    return Status;
  }

  void setTimeON( int datoC[2]){

     datoTON[0] = datoC[1];
     datoTON[1] = datoC[2];

     Serial.print("Set ON time to:");
     Serial.print(datoTON[0]);
     Serial.print(":");
     Serial.println(datoTON[1]);
  }

  void setTimeOFF( int datoC[2]){

     datoTOFF[0] = datoC[1];
     datoTOFF[1] = datoC[2];

     Serial.print("Set OFF time to:");
     Serial.print(datoTOFF[0]);
     Serial.print(":");
     Serial.println(datoTOFF[1]);
  }




////////////////////////////////////////////////////
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  BTS.Start();
  Acciones.init();
  motors1.setSpeed(0);

//////
 startWifi();

  initTime("COT5");   // Set for Melbourne/AU
  printLocalTime();
  
  pinMode(ledPin, OUTPUT);
  ledState == LOW;

     datoTON[0] = 16;
     datoTON[1] = 40;

     datoTOFF[0] = datoTON[0];
     datoTOFF[1] =  datoTON[1]+5;
////

}

void loop()
{
  ///////////////Timer///////////////////////////////////

    unsigned long currentMillis = millis();    // Se toma el tiempo actual

      // se comprueba si el tiempo actual menos el tiempo en que el LED cambió
      // de estado por última vez es mayor que el intervalo.
      if (currentMillis - previousMillis > intervalScan){

      // Si se cumple la condición se guarda el nuevo tiempo
      // en el que el LED cambia de estado
      previousMillis = currentMillis;

      // Y ahora cambiamos de estado el LED, si está encendido a
      // apagado o viceversa.
      if (ledState == LOW){
         datoTC[0] = datoTON[0];
         datoTC[1] = datoTON[1];
          if (Compare_Time(datoTC)==true){
            ledState=true;
          };
      }
    
      else if ((ledState == HIGH)){
         datoTC[0] = datoTOFF[0];
         datoTC[1] = datoTOFF[1];
          if (Compare_Time(datoTC)==true){
            ledState=false;
          };

      }
         

      // Hacemos que el contenido de la variable llegue al LED
      digitalWrite(ledPin, ledState);
      }


  ///////////////////////////////////////////////////////////////////////


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
    // El sistema se detiene completamente
    // Serial.println("   ////////////////   STOP ////////////////    ");
    // frenar(1);
    break;
  case 1:
    //Serial.println("Sistema bloques DATOS:   ");
    // Recibe();
    //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
    // act.movimientos((int)dato[1], (int)dato[2]);
    // frenar(10);
    motors1.setSpeed(0);
    break;

  case 2:
    //Serial.println("Sistema bloques DATOS:   ");
    // Recibe();
    //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
    // act.movimientos((int)dato[1], (int)dato[2]);
    // frenar(10);
    motors1.setSpeed(50);
    break;

    case 3:
    //Serial.println("Sistema bloques DATOS:   ");
    //bool Status_St=digitalRead(Switch);
    Acciones.home();
    datoT[0] = 0;

    break;


    case 4:
    //Serial.println("Sistema bloques DATOS:   ");
    //bool Status_St=digitalRead(Switch);
    printLocalTime();
    Serial.print("Set ON time to:");
     Serial.print(datoTON[0]);
     Serial.print(":");
     Serial.println(datoTON[1]);

     Serial.print("Set OFF time to:");
     Serial.print(datoTOFF[0]);
     Serial.print(":");
     Serial.println(datoTOFF[1]);
    datoT[0] = 0;

    break;

    case 5:
    setTimeON(datoT);
    datoT[0] = 0;

    break;

    case 6:
    setTimeOFF(datoT);
    datoT[0] = 0;
    break;

    case 7:
    //Serial.println("Sistema bloques DATOS:   ");
    //bool Status_St=digitalRead(Switch);

        // if(datoT[1]==1){
          
        //  datoTC[0] = datoTON[0];
        //  datoTC[1] = datoTON[1];
        // }
        // else if(datoT[1]==2){
        
        //  datoTC[0] = datoTOFF[0];
        //  datoTC[1] = datoTOFF[1];
        // }


        //  Serial.print(datoTC[0]);
        //  Serial.print(":");
        //  Serial.println(datoTC[1]);

    //Compare_Time(datoTC);

    ledState=HIGH;
    digitalWrite(ledPin, ledState);
    datoT[0] = 0;
    break;

    case 8:
    ledState=LOW;
    digitalWrite(ledPin, ledState);
     datoT[0] = 0;
    break;
    

    default:
    // colocar movimientos pero en cada instante
    Serial.println("Sistema tele operado");
    // act.tele((int)dato[1]);
    datoT[0] = 0;

    //Program Home


  }

}


