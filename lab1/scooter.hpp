#pragma once
#include <fstream>
#include <string>

static constexpr int MIN_PERCENTAGE = 30;
static constexpr double MAX_MILEAGE = 1500.0;

struct Statistics;

struct Scooter {
    std::string id;
    std::string model;
    int percentage;
    double mileage;
    int flagError;
};

void distributeScooters(const Scooter& scooter,
                        std::ofstream& out,
                        std::ofstream& out2,
                        std::ofstream& out3);

void updStat(const Scooter& scooter, Statistics& stat);

void printStatistics(const Statistics& stat);

struct Statistics {
    int total = 0;
    int ready = 0;
    int chargeRequired = 0;
    int serviceRequired = 0;
    double averageCharge = 0.0;
    double readyPercentage = 0.0;

};