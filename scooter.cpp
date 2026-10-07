#include "scooter.hpp"
#include <fstream>
#include <iostream>


void distributeScooters(const Scooter& scooter,
                        std::ofstream& out,
                        std::ofstream& out2,
                        std::ofstream& out3)
{
    if (scooter.flagError == 1) {
        out3 << scooter.id << ' ' << scooter.model << ' ' << scooter.percentage << ' ' << scooter.mileage << ' ' << scooter.flagError << std::endl;
    }
    else if (scooter.percentage < MIN_PERCENTAGE) {
        out2 << scooter.id << ' ' << scooter.model << ' ' << scooter.percentage << ' ' << scooter.mileage << ' ' << scooter.flagError << std::endl;
    }
    else if (scooter.mileage >= MAX_MILEAGE) {
        out3 << scooter.id << ' ' << scooter.model << ' ' << scooter.percentage << ' ' << scooter.mileage << ' ' << scooter.flagError << std::endl;
    }
    else {
        out << scooter.id << ' ' << scooter.model << ' ' << scooter.percentage << ' ' << scooter.mileage << ' ' << scooter.flagError << std::endl;
    }
}

void updStat(const Scooter& scooter, Statistics& stat) {
    stat.total++;
    stat.averageCharge += scooter.percentage;
    if (scooter.flagError == 1 || scooter.mileage >= MAX_MILEAGE) {
        stat.serviceRequired++;
    }
    else if (scooter.percentage < MIN_PERCENTAGE) {
        stat.chargeRequired++;
    }
    else {
        stat.ready++;
    }
    stat.readyPercentage = (static_cast<double>(stat.ready) / stat.total) * 100.0;
}

void printStatistics(const Statistics& stat) {
    std::cout << " " << stat.total << std::endl;
    std::cout << "Количество готовых к использованию самокатов: " << stat.ready << std::endl;
    std::cout << "Количество самокатов, требующих зарядки: " << stat.chargeRequired << std::endl;
    std::cout << "Количество самокатов, требующих обслуживания: " << stat.serviceRequired << std::endl;
    std::cout << "Средний уровень заряда: " << (stat.averageCharge / stat.total) << "%" << std::endl;
    std::cout << "Процент готовых к использованию самокатов: " << stat.readyPercentage << "%" << std::endl;
}