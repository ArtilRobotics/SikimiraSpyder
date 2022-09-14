#include <Arduino.h>
#include <KarakuriMotors.h>
#include <KarakuriBluetooth.h>

KarakuriMotors motors;
KarakuriBluetooth BTS;
//Henry 
String strT = "";
const char separatorT = ',';
const int dataLengthT = 3;
int datoT[dataLengthT];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  BTS.Start();
  // motors.setSpeeds(0,0);
}

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
    // El sistema se detiene completamente
    // Serial.println("   ////////////////   STOP ////////////////    ");
    // frenar(1);
    break;
  case 1:
    Serial.println("Sistema bloques DATOS:   ");
    // Recibe();
    //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
    // act.movimientos((int)dato[1], (int)dato[2]);
    // frenar(10);
    break;
  default:
    // colocar movimientos pero en cada instante
    Serial.println("Sistema tele operado");
    // act.tele((int)dato[1]);
    datoT[0] = 0;
    break;
  }
}