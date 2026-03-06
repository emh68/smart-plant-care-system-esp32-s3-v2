#include "WateringZone.h"

// Initialize basic hardware pins and names
WateringZone::WateringZone(uint8_t sensorPin, uint8_t pumpPin, uint8_t id, String name)
    : _sensorPin(sensorPin), _pumpPin(pumpPin), _id(id), _plantName(name)
{
    _dryValue = 520;   // Default 0% calibration
    _waterValue = 260; // Default 100% calibration
    _currentRaw = 0;
}

void WateringZone::begin()
{
    // Match ESP32-S3 to the 10-bit resolution from the manufacturer
    analogReadResolution(10);

    // Open unique storage folder based on ID
    String namespaceName = "zone" + String(_id);
    _prefs.begin(namespaceName.c_str(), false);

    // Load saved calibration or use defaults
    _dryValue = _prefs.getInt("dry", 520);
    _waterValue = _prefs.getInt("water", 260);
    _prefs.end();

    pinMode(_sensorPin, INPUT);
    pinMode(_pumpPin, OUTPUT);
}

void WateringZone::update()
{
    _currentRaw = analogRead(_sensorPin);
}

int WateringZone::getMoisturePercent()
{
    // Map inverted values: High (520) to 0%, Low (260) to 100%
    int moisturePercent = map(_currentRaw, _dryValue, _waterValue, 0, 100);
    return constrain(moisturePercent, 0, 100);
}

void WateringZone::saveCalibration(int dry, int water)
{
    _dryValue = dry;
    _waterValue = water;

    String namespaceName = "zone" + String(_id);
    _prefs.begin(namespaceName.c_str(), false);
    _prefs.putInt("dry", _dryValue);
    _prefs.putInt("water", _waterValue);
    _prefs.end();
}

void WateringZone::setPumpSpeed(int speed)
{
    // 0-255 PWM for motor speed control
    analogWrite(_pumpPin, speed);
}