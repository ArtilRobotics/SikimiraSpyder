#pragma once

#ifndef PermanentValues_h
#define PermanentValues_h

class PermanentValues
{
public:
    void init();
    void WR_distance(bool Position, int Value, bool Write);
    void WR_time(bool SetTime, int ValueHour, int ValueMin, bool Write);
    void WR_day(int Day, bool SetValue, bool Write);
    void WR_Sequence(int Sequence, int Period, bool Write);
    void WR_timeSquence();
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
    void getTime();
 
private:
};
#endif