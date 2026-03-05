#ifndef TEMP_HUM_SENSOR_H
#define TEMP_HUM_SENSOR_H

#include "Seeed_SHT35.h"

class TempHumSensor
{
private:
    SHT35 _sht35;
    // Caching temp + hum for efficiency
    float _currentTemp;
    float _currentHum;

public:
    TempHumSensor(int pinNum);
    bool init();
    void update(); // Retrieve new data from sensor (update cache)
    float getTemp();
    float getHum();
};

#endif