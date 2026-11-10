#include "weather.hpp"

#include <algorithm>
#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>

Celsius::Celsius(double value) : value_(value) {}

double Celsius::value() const {
    return value_;
}

Fahrenheit::Fahrenheit(double value) : value_(value) {}

double Fahrenheit::value() const {
    return value_;
}

std::vector<Measurement> readMeasurements(std::istream& input) {
    std::vector<Measurement> measurements;
    std::string line;
    std::getline(input, line); // Пропускаем заголовок CSV.

    while (std::getline(input, line)) {
        std::istringstream row(line);
        Measurement measurement{};
        std::string field;
        std::getline(row, measurement.time, ',');
        std::getline(row, field, ',');
        measurement.temperature = std::stod(field);
        std::getline(row, field, ',');
        measurement.humidity = std::stoi(field);
        std::getline(row, field, ',');
        measurement.pressure = std::stod(field);
        std::getline(row, field);
        measurement.wind = std::stof(field);
        measurements.push_back(measurement);
    }

    return measurements;
}

WeatherStatistics calculateStatistics(const std::vector<Measurement>& measurements) {
    const Measurement& first = measurements.front();
    WeatherStatistics statistics{
        first.temperature, first.temperature, 0.0,
        first.humidity, first.humidity, 0.0,
        first.pressure, first.pressure, 0.0,
        first.wind, first.wind, 0.0
    };
    const double count = static_cast<double>(measurements.size());

    for (const Measurement& measurement : measurements) {
        statistics.minTemperature = std::min(statistics.minTemperature, measurement.temperature);
        statistics.maxTemperature = std::max(statistics.maxTemperature, measurement.temperature);
        statistics.minHumidity = std::min(statistics.minHumidity, measurement.humidity);
        statistics.maxHumidity = std::max(statistics.maxHumidity, measurement.humidity);
        statistics.minPressure = std::min(statistics.minPressure, measurement.pressure);
        statistics.maxPressure = std::max(statistics.maxPressure, measurement.pressure);
        statistics.minWind = std::min(statistics.minWind, measurement.wind);
        statistics.maxWind = std::max(statistics.maxWind, measurement.wind);

        // Суммируем доли в double: целочисленного деления влажности нет.
        statistics.averageTemperature += measurement.temperature / count;
        statistics.averageHumidity += measurement.humidity / count;
        statistics.averagePressure += measurement.pressure / count;
        statistics.averageWind += measurement.wind / count;
    }

    return statistics;
}

void printValue(std::ostream& output, int value) {
    output << value;
}

void printValue(std::ostream& output, float value) {
    output << std::fixed << std::setprecision(1) << value;
}

void printValue(std::ostream& output, double value) {
    output << std::fixed << std::setprecision(2) << value;
}

Celsius toCelsius(Celsius temperature) {
    return temperature;
}

Celsius toCelsius(Fahrenheit temperature) {
    return Celsius((temperature.value() - 32.0) * 5.0 / 9.0);
}

Fahrenheit toFahrenheit(Celsius temperature) {
    return Fahrenheit(temperature.value() * 9.0 / 5.0 + 32.0);
}

double toKilometersPerHour(float metersPerSecond) {
    return metersPerSecond * 3.6;
}

void printReport(std::ostream& output, const std::vector<Measurement>& measurements) {
    if (measurements.empty()) {
        output << "Измерения отсутствуют.\n";
        return;
    }

    const WeatherStatistics statistics = calculateStatistics(measurements);
    output << "Суточный отчёт метеостанции\n"
           << "Количество измерений: " << measurements.size() << "\n\n"
           << "Показатель | Минимум | Максимум | Среднее\n"
           << "Температура, °C | ";
    printValue(output, statistics.minTemperature);
    output << " | ";
    printValue(output, statistics.maxTemperature);
    output << " | ";
    printValue(output, statistics.averageTemperature);
    output << "\nВлажность, % | ";
    printValue(output, statistics.minHumidity);
    output << " | ";
    printValue(output, statistics.maxHumidity);
    output << " | ";
    printValue(output, statistics.averageHumidity);
    output << "\nДавление, гПа | ";
    printValue(output, statistics.minPressure);
    output << " | ";
    printValue(output, statistics.maxPressure);
    output << " | ";
    printValue(output, statistics.averagePressure);
    output << "\nВетер, м/с | ";
    printValue(output, statistics.minWind);
    output << " | ";
    printValue(output, statistics.maxWind);
    output << " | ";
    printValue(output, statistics.averageWind);

    const std::size_t shownCount = std::min(measurements.size(), std::size_t{5});
    output << "\n\nПервые " << shownCount << " измерений:\n"
           << "Время | °C | °F | Влажность, % | Давление, гПа | Ветер, м/с | Ветер, км/ч\n";
    for (std::size_t index = 0; index < shownCount; ++index) {
        const Measurement& measurement = measurements[index];
        const Celsius temperature(measurement.temperature);
        output << measurement.time << " | ";
        printValue(output, temperature.value());
        output << " | ";
        printValue(output, toFahrenheit(temperature).value());
        output << " | ";
        printValue(output, measurement.humidity);
        output << " | ";
        printValue(output, measurement.pressure);
        output << " | ";
        printValue(output, measurement.wind);
        output << " | ";
        printValue(output, toKilometersPerHour(measurement.wind));
        output << '\n';
    }

    output << "\nПреобразование Фаренгейта в Цельсий: ";
    const Fahrenheit example(68.0);
    printValue(output, example.value());
    output << " °F = ";
    printValue(output, toCelsius(example).value());
    output << " °C\nПреобразование Цельсия в Цельсий: ";
    const Celsius firstTemperature(measurements.front().temperature);
    printValue(output, firstTemperature.value());
    output << " °C = ";
    printValue(output, toCelsius(firstTemperature).value());
    output << " °C\n";

    const double temperature = firstTemperature.value();
    if (temperature >= std::numeric_limits<int>::min() &&
        temperature <= std::numeric_limits<int>::max()) {
        output << "Целая часть первой температуры (к нулю): ";
        printValue(output, static_cast<int>(temperature));
        output << " °C\n";
    } else {
        output << "Первая температура вне диапазона int: преобразование не выполнено.\n";
    }
}
