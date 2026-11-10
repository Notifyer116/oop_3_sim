#include "weather.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    const std::string filename = argc > 1 ? argv[1] : "weather_day.csv";
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << '\n';
        return 1;
    }

    const std::vector<Measurement> measurements = readMeasurements(input);
    printReport(std::cout, measurements);
    return 0;
}
