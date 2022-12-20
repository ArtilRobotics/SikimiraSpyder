#include <KarakuriBluetooth.h>
#include <Arduino.h>
#include <Actions.h>
//#include <BluetoothSerial.h>

#include <SoftwareSerial.h>
SoftwareSerial BT(8, 9); // RX | TX

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif

Actions act;
//BluetoothSerial BT;
// variables para la comunicación usando función
String str = "";
const char separator = ',';
const int dataLength = 3;
int dato[dataLength];
int cont=0;
bool ActivityStatus = true;
bool SliderStatus = false;

// Definición de la bandera de estados del sistema
int estado = 0;
long previousTime = 0;
long intervalScanTime = 2000;
long previousTime2 = 0;
long intervalScanTime2 = 500;
long previousSecs = 0;
long intervalScanTimeSecs = 30;
long previousMins = 0;
long intervalScanTimeMin = 3;
long previousHours = 0;
long intervalScanTimeHour = 1;
unsigned long previousMillis3 = 0;
unsigned long interval3 = 30000;

// long previousMillis2 = 0;
// long intervalScan2 = 100;

//  act.SpiderDayState=true;
//  act.DeviceTimeStatus=true;

///////////////////////

void KarakuriBluetooth::Start()
{
    // BT.begin("Sikimira");
    BT.begin(115200);
    Serial.println("BT iniciado");
    int Secuencia_act=act.Sequence_Update();
    
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
            Serial.print("Dato "); 
            Serial.print(i);
            Serial.print("="); 
            Serial.print(dato[i]);
        }
        Serial.println(" ");
    }

    switch ((int)dato[0])
    {

    case 0:

        dato[0] = 0;
        break;


    case 1:
        serialFlush();
        act.MoveSpyder(dato[1]);
        dato[0] = 0;
        break;

    case 2: // Configurar Set Alto
        Serial.print("Distance HIGH recieved: ");
        Serial.println(dato[1]);

        act.setHigh(dato[1]);
        dato[0] = 0;
        break;

    case 3: // Configurar Set Bajo
        Serial.print("Distance LOW recieved: ");
        Serial.println(dato[1]);

        act.setLow(dato[1]);
        dato[0] = 0;
        break;

    case 4:

       
        dato[0] = 0;
        break;

    case 5:
       
        dato[0] = 0;
        break;

    case 6: // Setear dias Activos
        
        dato[0] = 0;

        break;

    case 7: //set Sequence

        if (dato[1]==1){
            SliderStatus=true;
            Serial.println("Slider Status: ON");
        }
        else {SliderStatus=false;
            Serial.println("Slider Status: OFF");}
        
        dato[0] = 0;

        break;

    case 8: //set Sequence

        act.SetSequence(dato[1]);
        // act.PrintTimes();
        dato[0] = 0;

        break;

    case 9: // Boton Home

        // Funcion de prueba usada para verificar Seteto y obtencion de Informacion Permanente
        //  act.getTimesProgram();
        //  act.getLevels();
        //  act.PrintTimes();
        //  act.PrintLevels();

        // Funcion Original
        act.home();
        act.home();
        dato[0] = 0;
        break;

    case 10:
        // ESP.restart();
        dato[0] = 0;
        break;

    case 11:
        act.SetPeriodSequence(dato[1]);
        act.Sequence_Update();
        dato[0] = 0;
        break;

    default:
        // colocar movimientos pero en cada instante
        Serial.println("Sistema tele operado");
        // act.tele((int)dato[1]);
        serialFlush();
        dato[0] = 0;
        ti = 0;
        break;
    }

    if(SliderStatus != true){
    if(act.SpiderDayState){
    if (act.DeviceTimeStatus)
    {

        
        if (act.Period>0)
        {
        unsigned long currentMins = (millis()/1000)/60;                  // Medir en Minutos
        if (currentMins - previousMins > act.Period-1)
        {
            Serial.println("Activated Loop cada: ");
            Serial.print(act.Period);
            Serial.println("Mins ");
            Serial.print("Secuencia activada: ");
            int Secuencia_act=act.Sequence_Update();
            Serial.println(Secuencia_act);
            act.movimientos(Secuencia_act, 500);
            previousMins = currentMins;
        }
        }

        if (act.Period==0)
        {
            unsigned long currentTime = millis(); // Medir en millis
            if (currentTime - previousTime >= intervalScanTime){
                Serial.println("Activated Loop cada 2s");
                Serial.print("Secuencia activada: ");
                int Secuencia_act=act.Sequence_Update();
                Serial.println(Secuencia_act);
                act.movimientos(Secuencia_act, 500);
                previousTime = currentTime;
                }
         }
    }
    }
    }

    unsigned long currentTime2 = millis(); // Medir en millis

    if (currentTime2 - previousTime2 > intervalScanTime2)
    {
        act.CheckLenght();
        previousTime2 = currentTime2;
    }

      unsigned long currentMillis3 = millis();


}

