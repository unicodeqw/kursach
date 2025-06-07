#include "FuelTank.h"
#include <algorithm> // for std::min

FuelTank::FuelTank(std::string type, double maxVol, double currentVol)
    : fuelType(std::move(type)), maxVolume(maxVol), currentVolume(currentVol) {}

bool FuelTank::getFuel(double amount) {
    if (amount > 0 && currentVolume >= amount) {
        currentVolume -= amount;
        return true;
    }
    return false;
}

void FuelTank::addFuel(double amount) {
    if (amount > 0) {
        currentVolume = std::min(currentVolume + amount, maxVolume);
    }
}

double FuelTank::getVolume() const { return currentVolume; }
double FuelTank::getMaxVolume() const { return maxVolume; }
std::string FuelTank::getFuelType() const { return fuelType; }

double FuelTank::getVolumePercentage() const {
    if (maxVolume == 0) return 0.0;
    return (currentVolume / maxVolume) * 100.0;
}