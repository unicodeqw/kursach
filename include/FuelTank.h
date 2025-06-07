#pragma once
#include <string>

class FuelTank {
public:
    FuelTank(std::string type, double maxVol, double currentVol);

    bool getFuel(double amount);
    void addFuel(double amount);

    double getVolume() const;
    double getMaxVolume() const;
    std::string getFuelType() const;
    double getVolumePercentage() const;

private:
    std::string fuelType;
    double currentVolume;
    double maxVolume;
};