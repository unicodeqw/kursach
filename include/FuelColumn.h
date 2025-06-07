#pragma once
#include <string>

class FuelTank;

class FuelColumn {
public:
    FuelColumn(int colId, FuelTank* tankPtr);

    int getId() const;
    std::string getFuelType() const;
    bool isBusy() const;
    void startService();
    void endService();
    bool dispenseFuel(double amount);

private:
    int id;
    FuelTank* tank; // Вказівник на резервуар
    bool busy;
};