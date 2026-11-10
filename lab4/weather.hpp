#pragma once

#include <iosfwd>
#include <string>
#include <vector>

struct Measurement {
    std::string time;
    double temperature;
    int humidity;
    double pressure;
    float wind;
};

class Celsius {
public:
    explicit Celsius(double value);
    double value() const;

private:
    double value_;
};

class Fahrenheit {
public:
    explicit Fahrenheit(double value);
    double value() const;

private:
    double value_;
};

struct WeatherStatistics {
    double minTemperature, maxTemperature, averageTemperature;
    int minHumidity, maxHumidity;
    double averageHumidity;
    double minPressure, maxPressure, averagePressure;
    float minWind, maxWind;
    double averageWind;
};

std::vector<Measurement> readMeasurements(std::istream& input);
// Вызывается только для непустого набора измерений.
WeatherStatistics calculateStatistics(const std::vector<Measurement>& measurements);

void printValue(std::ostream& output, int value);
void printValue(std::ostream& output, float value);
void printValue(std::ostream& output, double value);

Celsius toCelsius(Celsius temperature);
Celsius toCelsius(Fahrenheit temperature);
Fahrenheit toFahrenheit(Celsius temperature);
double toKilometersPerHour(float metersPerSecond);

void printReport(std::ostream& output, const std::vector<Measurement>& measurements);
