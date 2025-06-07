#include "FuelColumn.h"
#include "FuelTank.h"

FuelColumn::FuelColumn(int colId, FuelTank* tankPtr)
    : id(colId), tank(tankPtr), busy(false) {}

int FuelColumn::getId() const { return id; }

std::string FuelColumn::getFuelType() const {
    return tank ? tank->getFuelType() : "N/A";
}

bool FuelColumn::isBusy() const { return busy; }
void FuelColumn::startService() { busy = true; }
void FuelColumn::endService() { busy = false; }

bool FuelColumn::dispenseFuel(double amount) {
    if (tank) {
        return tank->getFuel(amount);
    }
    return false;
}