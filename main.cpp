#include <iostream>
#include <sstream>
#include <fstream>

#include "scooter.hpp"

int main() {
    std::ifstream input("data.txt");
    if (!input.is_open()) {
        std::cerr << "Ошибка открытия файла data.txt" << std::endl;
    }

    std::ofstream out("ready.txt", std::ios::app);
    std::ofstream out2("charge_required.txt", std::ios::app);
    std::ofstream out3("service_required.txt", std::ios::app);
    if (!out || !out2 || !out3) {
        std::cerr << "Ошибка создания файлов." << std::endl;
        return 1;
    }

    std::string line;
    Statistics stat;
    while (std::getline(input, line)) {
        std::istringstream stream(line);
        Scooter scooter;
        if (stream >> scooter.id >> scooter.model >> scooter.percentage >> scooter.mileage >> scooter.flagError) {
            distributeScooters(scooter, out, out2, out3);
            updStat(scooter, stat);
        } else {
            std::cerr << "Не удалось распарсить строчку." << std::endl;
        }
    }
    printStatistics(stat);
    return 0;
}