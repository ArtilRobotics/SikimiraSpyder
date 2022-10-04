#include <KarakuriBluetooth.h>
#include <Arduino.h>
#include <Actions.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

Actions act;
BluetoothSerial BT;
// variables para la comunicación usando función
String str = "";
const char separator = ',';
const int dataLength = 3;
int dato[dataLength];

// Definición de la bandera de estados del sistema
int estado = 0;

void KarakuriBluetooth::Start()
{
    BT.begin("Sikimira");
    BT.begin(9600);
    Serial.println("BT iniciado");
}

int ti = 0;

void serialFlush()
{
    while (BT.available() > 0)
    {
        char t = BT.read();
    }
}

void KarakuriBluetooth::Update()
{
    str = "";
    if (BT.available())
    {
        str = BT.readStringUntil('\n');
        Serial.println(str);
        for (int i = 0; i < dataLength; i++)
        {
            int index = str.indexOf(separator);
            dato[i] = str.substring(0, index).toInt();
            str = str.substring(index + 1);
        }
        for (int i = 0; i < dataLength; i++)
        {
            Serial.printf("Dato %d = %d  ", i, dato[i]);
        }
        Serial.println(" ");
    }
    switch ((int)dato[0])
    {
    case 0:
        // El sistema se detiene completamente
        // Serial.println("   ////////////////   STOP ////////////////    ");
        // frenar(1);
        // BT.println("S");
        break;
    case 1:
        Serial.println("Sistema bloques DATOS:   ");
        // Recibe();
        //  Aquí se pone la funcione de movimientos en la cual dependiendo de cual sea selecciona que realiza el carrito
        //act.movimientos((int)dato[1], (int)dato[2]);
        // frenar(10);
        ti++;
        BT.println(ti);
        delay(20);
        BT.flush();
        Serial.print("Bluetooth............... ");
        Serial.println(ti);
        dato[0] = 0;
        if (ti >= 18)
        {
            Serial.println("                   Fin");
            ti = 0;
            serialFlush();
        }
        break;
    default:
        // colocar movimientos pero en cada instante
        Serial.println("Sistema tele operado");
        //act.tele((int)dato[1]);
        serialFlush();
        dato[0] = 0;
        ti = 0;
        break;
    }
}
