#include <PermanentValues.h>
//#include <Preferences.h>
#include <Arduino.h>
#include <EEPROM.h> 

//Preferences preferences;

int DefaultHigh = 100;
char ValueHourDef = 14;
char ValueMinDef = 22;
int DefaultLow = 0;
int ValueHigh;
int ValueLow;
int ValueHourON;
int ValueMinON;
int ValueHourOFF;
int ValueMinOFF;
int Sequence_Select;
int PeriodSequence;
bool dayStatus;
bool dayConfig[7];

void PermanentValues::WR_distance(bool Position, int Value, bool Write)
{

    // preferences.begin("PermValues", false);

    // Write is true for "Write" State;
    // Write is false for "Read" State;

    if (Write)
    {
        // Position is true for "HIGH" State;
        if (Position == true)
        {
            EEPROM.put(0, Value);
            //preferences.putUInt("SetHIGH", Value);
        }
        // Position is false for "LOW" State;
        else if (Position == false)
        {
            EEPROM.put(1, Value);
            //preferences.putUInt("SetLOW", Value);
        }
    }

    if (Write == false)
    {
        // Position is true for "HIGH" State;
        if (Position == true)
        {   
            EEPROM.get(0, ValueHigh);
            //ValueHigh = preferences.getUInt("SetHIGH", DefaultHigh);
        }
        // Position is false for "LOW" State;
        else if (Position == false)
        {   
            EEPROM.get(1, ValueLow);
           //ValueLow = preferences.getUInt("SetLOW", DefaultLow);
        }
    }

    //preferences.end();
}

void PermanentValues::WR_time(bool SetTime, int ValueHour, int ValueMin, bool Write)
{

    // preferences.begin("PermValues", false);

    // // Write is true for "Write" State;
    // // Write is false for "Read" State;

    // if (Write)
    // {
    //     // SetTime is true for "TimeON" ;
    //     if (SetTime == true)
    //     {
    //         preferences.putUChar("TimeONHour", ValueHour);
    //         preferences.putUChar("TimeONMin", ValueMin);
    //     }
    //     // SetTime is false for "TimeOFF";
    //     else if (SetTime == false)
    //     {
    //         preferences.putUChar("TimeOFFHour", ValueHour);
    //         preferences.putUChar("TimeOFFMin", ValueMin);
    //     }
    // }

    // if (Write == false)
    // {

    //     // SetTime is true for "TimeON" ;
    //     if (SetTime == true)
    //     {
    //         ValueHourON = preferences.getUChar("TimeONHour", ValueHourDef);
    //         ValueMinON = preferences.getUChar("TimeONMin", ValueMinDef);
    //     }
    //     // SetTime is true for "TimeOFF" ;
    //     else if (SetTime == false)
    //     {
    //         ValueHourOFF = preferences.getUChar("TimeOFFHour", ValueHourDef);
    //         ValueMinOFF = preferences.getUChar("TimeOFFMin", ValueMinDef);
    //     }
    // }

    // preferences.end();
}

void PermanentValues::WR_day(int Day, bool SetValue, bool Write)
{

    // preferences.begin("PermValues", false);

    // if (Write)
    // {

    //     if (Day == 1)
    //     {
    //         preferences.putBool("Monday", SetValue);
    //         Serial.print("Set Monday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 2)
    //     {
    //         preferences.putBool("Tuesday", SetValue);
    //         Serial.print("Set Tuesday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 3)
    //     {
    //         preferences.putBool("Wednesday", SetValue);
    //         Serial.print("Set Wednesday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 4)
    //     {
    //         preferences.putBool("Thursday", SetValue);
    //         Serial.print("Set Thursday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 5)
    //     {
    //         preferences.putBool("Friday", SetValue);
    //         Serial.print("Set Friday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 6)
    //     {
    //         preferences.putBool("Saturday", SetValue);
    //         Serial.print("Set Saturday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }

    //     else if (Day == 0)
    //     {
    //         preferences.putBool("Sunday", SetValue);
    //         Serial.print("Set Sunday Status: ");
    //         if (SetValue == true)
    //         {
    //             Serial.println("ON");
    //         }
    //         if (SetValue == false)
    //         {
    //             Serial.println("OFF");
    //         }
    //     }
    // }

    // if (Write == false)
    // {

    //     if (Day == 1)
    //     {
    //         dayStatus = preferences.getBool("Monday", SetValue);
    //     }

    //     else if (Day == 2)
    //     {
    //         dayStatus = preferences.getBool("Tuesday", SetValue);
    //     }

    //     else if (Day == 3)
    //     {
    //         dayStatus = preferences.getBool("Wednesday", SetValue);
    //     }

    //     else if (Day == 4)
    //     {
    //         dayStatus = preferences.getBool("Thursday", SetValue);
    //     }

    //     else if (Day == 5)
    //     {
    //         dayStatus = preferences.getBool("Friday", SetValue);
    //     }

    //     else if (Day == 6)
    //     {
    //         dayStatus = preferences.getBool("Saturday", SetValue);
    //     }

    //     else if (Day == 0)
    //     {
    //         dayStatus = preferences.getBool("Sunday", SetValue);
    //     }


    // }

    // preferences.end();

}

void PermanentValues::WR_Sequence(int Sequence, int Period ,bool Write)
{

    //preferences.begin("PermValues", false);

    if (Write)
    {

        if (Sequence == 1)
        {
            EEPROM.put(2, true);
            EEPROM.put(3, false);
            EEPROM.put(4, false);
            EEPROM.put(5, false);
            // preferences.putBool("Sequence1", true);
            // preferences.putBool("Sequence2", false);
            // preferences.putBool("Sequence3", false);
            // preferences.putBool("Random", false);
            Serial.println("Set Sequence1 Status: ON");
        }

        else if (Sequence == 2)
        {
            EEPROM.put(2, false);
            EEPROM.put(3, true);
            EEPROM.put(4, false);
            EEPROM.put(5, false);
            // preferences.putBool("Sequence1", false);
            // preferences.putBool("Sequence2", true);
            // preferences.putBool("Sequence3", false);
            // preferences.putBool("Random", false);
            Serial.println("Set Sequence 2 Status: ON");
        }

        else if (Sequence == 3)
        {
            EEPROM.put(2, false);
            EEPROM.put(3, false);
            EEPROM.put(4, true);
            EEPROM.put(5, false);
        //    preferences.putBool("Sequence1", false);
        //     preferences.putBool("Sequence2", false);
        //     preferences.putBool("Sequence3", true);
        //     preferences.putBool("Random", false);
            Serial.println("Set Sequence 3 Status: ON");
        }

        else if (Sequence == 4)
        {
            EEPROM.put(2, false);
            EEPROM.put(3, false);
            EEPROM.put(4, false);
            EEPROM.put(5, true);
            // preferences.putBool("Sequence1", false);
            // preferences.putBool("Sequence2", false);
            // preferences.putBool("Sequence3", false);
            // preferences.putBool("Random", true);
            Serial.println("Set Sequence 4 Status: ON");
        }

        else if (Sequence == 0)
        {
            
            EEPROM.put(6, false);
            //preferences.putUChar("PeriodT", Period);
            Serial.print("Set Period Sequence on:");
            Serial.println(Period);
        }


    }

    if (Write == false)
    {
        bool StatusSequence1;
        bool StatusSequence2;
        bool StatusSequence3;
        bool StatusSequence4;
        

            EEPROM.get(2, StatusSequence1);
            EEPROM.get(3, StatusSequence2);
            EEPROM.get(4, StatusSequence3);
            EEPROM.get(5, StatusSequence4);

        //if (preferences.getBool("Sequence1", false))
        if (StatusSequence1)
        {
            Sequence_Select = 1;
        }

        //else if (preferences.getBool("Sequence2", false))
        else if (StatusSequence2)
        {
            Sequence_Select = 2;
        }

       //else if (preferences.getBool("Sequence3", false))
        else if (StatusSequence3)
        {
            Sequence_Select = 3;
        }

        //else if (preferences.getBool("Random", false))
        else if (StatusSequence4)
        {
            Sequence_Select = 4;
        }

        //PeriodSequence = preferences.getUChar("PeriodT", 0);

    }

    //preferences.end();
}

void PermanentValues::getTime()
{
    // preferences.begin("PermValues", false);

    // ValueHourON = preferences.getUChar("TimeONHour", ValueHourDef);
    // ValueMinON = preferences.getUChar("TimeONMin", ValueMinDef);
    // ValueHourOFF = preferences.getUChar("TimeOFFHour", ValueHourDef);
    // ValueMinOFF = preferences.getUChar("TimeOFFMin", ValueMinDef);
    // dayConfig[0] = preferences.getBool("Monday", 0);
    // dayConfig[1] = preferences.getBool("Tuesday", 0);
    // dayConfig[2] = preferences.getBool("Wednesday", 0);
    // dayConfig[3] = preferences.getBool("Thursday", 0);
    // dayConfig[4] = preferences.getBool("Friday", 0);
    // dayConfig[5] = preferences.getBool("Saturday", 0);
    // dayConfig[6] = preferences.getBool("Sunday", 0);
    // WR_Sequence(0, 0 ,false);
    
    // preferences.end();
}

void PermanentValues::init()
{
    // preferences.begin("PermValues", false);
    // getTime();
    EEPROM.put(0, DefaultHigh);
    EEPROM.put(1, DefaultLow);
}
