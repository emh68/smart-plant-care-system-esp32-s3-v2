#include "LightSensor.h"

bool LightSensor::begin()
{
    if (!_as7341.begin())
        return false;
    _as7341.setATIME(100);
    _as7341.setASTEP(999);
    _as7341.setGain(AS7341_GAIN_256X);
    return true;
}

void LightSensor::update()
{
    if (_as7341.readAllChannels())
    {
        for (int i = 0; i < 8; i++)
        {
            _readings[i] = _as7341.getChannel((as7341_color_channel_t)_spectrumChannels[i]);
        }
        _readings[8] = _as7341.getChannel(AS7341_CHANNEL_CLEAR);
        _readings[9] = _as7341.getChannel(AS7341_CHANNEL_NIR);
    }
}

uint16_t LightSensor::getChannelValue(int index)
{
    return _readings[index];
}