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

bool ActivityStatus = true;

// Definición de la bandera de estados del sistema
int estado = 0;

long previousTime = 0;
long intervalScanTime = 2000;
long previousTime2 = 0;
long intervalScanTime2 = 500;
long previousMins = 0;
long intervalScanTimeMin = 30000;
long previousHours = 0;
long intervalScanTimeHour = 1;

// long previousMillis2 = 0;
// long intervalScan2 = 100;

///////////////////////

void KarakuriBluetooth::Start()
{
    BT.begin("Sikimira");
    BT.begin(115200);
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

    // unsigned long currentMins = ( millis()/1000 ) / 60;    //Medir en Minutos
    unsigned long currentMins = millis();                       // Medir en Minutos
    unsigned long currentHours = ((millis() / 1000) / 60) / 60; // Medir en Horas

    if (currentMins - previousMins > intervalScanTimeMin)
    {
        act.GetTimeNow();
        act.CheckTimer();
        previousMins = currentMins;
    }

    if (currentHours - previousHours > intervalScanTimeHour)
    {
        // act.CheckDayStatus();
        previousHours = currentHours;
    }

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

        dato[0] = 0;
        break;

    case 10:
        if (ActivityStatus)
        {
            act.movimientos(20, 2);
        }
        dato[0] = 0;
        break;

    case 1:
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

        Serial.print("Time recieved: ");
        Serial.print(dato[1]);
        Serial.print(":");
        Serial.print(dato[2]);

        act.setON(dato);
        dato[0] = 0;
        break;

    case 5:
        Serial.print("Time recieved: ");
        Serial.print(dato[1]);
        Serial.print(":");
        Serial.print(dato[2]);

        act.setOFF(dato);
        dato[0] = 0;
        break;

    case 6: // Setear dias Activos
        // El contenido de entrada serial es del tipo: 6,domingo,true
        // Es mejor opcion cambiar al tipo de formato: 6,6,1;
        // Para poder usar correctamente la funcion PermanentValues::WR_day

        // //usar el formato para los dos valores siguientes del Serial de esta forma:
        // Lunes = 0
        // Martes = 1
        // Miercoles = 2
        // Jueves = 3
        // Viernes = 4
        // Sabado = 5
        // Domingo = 6

        // El tercer valor corresponde a el booleano true = 1 y false =  0

        act.setDays(dato); // FUNCION ORIGINAL

        // act.atras(0); //Prueba de conteo
        // act.movimientos(20,2);
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

    // if(act.SpiderDayState){
    if (act.DeviceTimeStatus)
    {

        unsigned long currentTime = millis(); // Medir en millis

        if (currentTime - previousTime > intervalScanTime)
        {
            // act.GetTimeNow();
            Serial.println("Activated Loop");
            previousTime = currentTime;
        }
    }
    // }

    unsigned long currentTime2 = millis(); // Medir en millis

    if (currentTime2 - previousTime2 > intervalScanTime2)
    {
        act.CheckLenght();
        previousTime2 = currentTime2;
    }
}
