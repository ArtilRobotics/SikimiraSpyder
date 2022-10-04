#include <Arduino.h>
#include <KarakuriMotors.h>
#include <KarakuriBluetooth.h>
#include <Actions.h>
#include <TimeClock.h>
//

#include <WiFi.h>



Actions Acciones;
KarakuriMotors motors1;
KarakuriBluetooth BTS;
TimeClock WifiTime;

/////// Inicio de datos para conexión WiFi por BT

//const char * ssid="Ortiz";
//const char * wifipw="22a";

#include "BluetoothSerial.h"
#include <EEPROM.h>
BluetoothSerial ESP_BT; //Object for Bluetooth
String buffer_in;
unsigned long previousMillisBT = 0; 
byte val;       
int addr = 0;
byte indS=0;
byte indP=0;
String stream;
byte len=0;
String temp;
String temp2;
unsigned int interval=60000;

/////// Fin de datos para conexión WiFi por BT

String strT = "";
const char separatorT = ',';
const int dataLengthT = 3;
int datoT[dataLengthT];

int ledState = LOW; 
int ledPin = 2 ;


long previousMillis = 0;
long intervalScan = 1000; 
//////////////////////////////////////////////////


long previousMillis2 = 0;
  long intervalScan2 = 500; 

long previousMillis3 = 0;
  long intervalScan3 = 1000; 


  long previousMillis4 = 0;
  long intervalScan4 = 200; 

long max_lenght;

/*
void  startWifi(){
  WiFi.begin(ssid, wifipw);
  Serial.println("Connecting Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Wifi RSSI=");
  Serial.println(WiFi.RSSI());
}
*/

//// Inicio de conexión WiFi por BT

boolean check_wifiUpdate(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisBT >= interval) {
    previousMillisBT = currentMillis;
    Serial.println("60 Seconds Over");
    return true;
  }
  else if (ESP_BT.available()){ //Check if we receive anything from Bluetooth
    interval=50000;
    buffer_in = ESP_BT.readStringUntil('\n'); //Read what we recevive 
    //Serial.println("Received:"); Serial.println(buffer_in);
    delay(20);
    if(buffer_in.charAt(0)=='S'){
      for(int i=0;i<buffer_in.length();i++){
        val=(byte)(buffer_in.charAt(i));
        //Serial.println("val "+val);
        EEPROM.write(addr, val);
        //Serial.println(val);
        addr++;
      }
      //Serial.print("New ");
      //Serial.print(buffer_in);
      EEPROM.write(addr, 10);
      addr++;
      EEPROM.commit();     
      ESP_BT.println("SSID Stored");
    }
    else if(buffer_in.charAt(0)=='P'){
      for(int i=0;i<buffer_in.length();i++){
        val=(byte)(buffer_in.charAt(i));
        //Serial.println("val "+val);
        EEPROM.write(addr, val);
        //Serial.println(val);
        addr++;
      }
      //Serial.print("New ");
      //Serial.print(buffer_in);
      EEPROM.write(addr, 10);
      EEPROM.commit();  
      ESP_BT.println("Password Stored"); 
      return true;
    }  
    return false;
  }
  else
  { return false;
  }
}

//// Fin de conexión WiFi por BT

//////////////////////////////////////////////////////
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTS.Start();
  Acciones.init();
  motors1.setSpeed(0);
  WifiTime.init();
//////
 

  
  
  pinMode(ledPin, OUTPUT);
  ledState == LOW;

     WifiTime.datoTON[0] = 16;
     WifiTime.datoTON[1] = 40;

     WifiTime.datoTOFF[0] = WifiTime.datoTON[0];
     WifiTime.datoTOFF[1] =  WifiTime.datoTON[1]+5;
/////// Inicio de conexión WiFi por BT

//startWifi();

EEPROM.begin(50);
// Serial.begin(9600); //Start Serial monitor in 9600
Serial.println("Bluetooth Device is Ready to Pair");
Serial.println("Waiting For Wifi Updates 30 seconds");
ESP_BT.begin("Sikimira"); //Name of your Bluetooth Signal

while(!check_wifiUpdate()==true){
  }
  Serial.println("The Stored Wifi credetial are : ");
  for(int i=0;i<50;i++){
    val=EEPROM.read(i);
    stream+=(char)val;
    if((val==10) && (indS==0)){
      indS=i;
      //Serial.println("indS"+(String)i);
    }
    else if(val==10 && indP==0){
      indP=i;
      break;
      //Serial.println("indP"+(String)i);
    }
  }
  // Serial.println(stream);
  // Serial.println("Stream Ended");
  temp=stream.substring(0,indS);
  temp=temp.substring(5,indS);
  //ssid2=ssid;
  temp2=stream.substring(indS+1,indP);
  temp2=temp2.substring(5,indP-indS);
  int i=temp.length();
  int j=temp2.length();
  char ssid[i];
  char pass[j]; 
  temp.toCharArray(ssid,i);
  temp2.toCharArray(pass,j);
  Serial.println("Stored SSID");
  Serial.println(ssid);
  Serial.println("Stored PASS");
  Serial.println(pass);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed");
    while(1) {
      delay(1000);
    }
  }
  else{
    Serial.print("Wifi Connected to ");
    Serial.println(ssid);
    digitalWrite(ledPin, HIGH);
  }

initTime("COT5");   // Set for Melbourne/AU
  printLocalTime();
///////  Fin de conexión WiFi por BT

}

////////////////////////////////////////////////////////////////

void loop()
{
  if((motors1.lenght<=0 && motors1.directionM==false) || digitalRead(Acciones.Switch)== LOW){
    motors1.setSpeed(0);
    motors1.lenght=0;
   
  }

  else if(motors1.lenght>max_lenght && motors1.directionM){
    motors1.setSpeed(0);
  }



  unsigned long currentMillis2 = millis();    // Se toma el tiempo actual

 
      if (currentMillis2 - previousMillis2 > intervalScan2){

       previousMillis2 = currentMillis2;
 
        motors1.speedSpyder(intervalScan2);
        
      }

 unsigned long currentMillis3 = millis();    // Se toma el tiempo actual
      
      
      if (currentMillis3 - previousMillis3 > intervalScan3){
      previousMillis3 = currentMillis3;
      
      Serial.print("Las revoluciones son: ");
      Serial.print(motors1.revs);
      Serial.print(" La Posicion es: ");
      Serial.println(motors1.lenght);
      //Serial.println(digitalRead(Button));
      }


 unsigned long currentMillis4 = millis();    // Se toma el tiempo actual

 
      if (currentMillis4 - previousMillis4 > intervalScan4){

        previousMillis4 = currentMillis4;

        motors1.moveSpyder(intervalScan4, motors1.directionM);
  
      }

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
    datoT[0] = 0;
    break;
  case 1:
    //Serial.println("Sistema bloques DATOS:   ");
    // Recibe();
    //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
    // act.movimientos((int)dato[1], (int)dato[2]);
    // frenar(10);
    motors1.setSpeed(0);
    datoT[0] = 0;
    break;

  case 2:
    //Serial.println("Sistema bloques DATOS:   ");
    // Recibe();
    //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
    // act.movimientos((int)dato[1], (int)dato[2]);
    // frenar(10);
    Serial.println(datoT[1]);
    motors1.setSpeed(datoT[1]);

    if(datoT[1]>0){motors1.directionM=true;}
    if(datoT[1]<0){motors1.directionM=false;}


    datoT[0] = 0;
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
    WifiTime.printLocalTime();
    Serial.print("Set ON time to:");
     Serial.print(WifiTime.datoTON[0]);
     Serial.print(":");
     Serial.println(WifiTime.datoTON[1]);

     Serial.print("Set OFF time to:");
     Serial.print(WifiTime.datoTOFF[0]);
     Serial.print(":");
     Serial.println(WifiTime.datoTOFF[1]);

    Serial.println(motors1.velocitysmoothed_R[1]);

    datoT[0] = 0;

    break;

    case 5:
    WifiTime.setTimeON(datoT);
    datoT[0] = 0;

    break;

    case 6:
    WifiTime.setTimeOFF(datoT);
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
    
    case 9:
    max_lenght=datoT[1];
    Serial.print("Longitud Maxima: ");
    Serial.println(max_lenght);

     datoT[0] = 0;
    break;

    case 10:
    Serial.print("Longitud motors1.directionM: ");
    Serial.println(motors1.directionM);
     datoT[0] = 0;
    break;

    default:
    // colocar movimientos pero en cada instante
    Serial.println("Sistema tele operado");
    // act.tele((int)dato[1]);
    datoT[0] = 0;

  }

}
//dsfsd


