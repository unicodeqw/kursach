#pragma once
#include "FuelTank.h"
#include "FuelColumn.h"
#include "Transaction.h"
#include <vector>
#include <map>
#include <string>
#include <optional>

class GasStation {
public:
    GasStation();

    bool serveCustomerByLiters(const std::string& fuelType, double liters);
    bool serveCustomerBySum(const std::string& fuelType, double sum);

    void printTankStatus() const;
    void printDailyReport() const;
    std::vector<std::string> getAvailableFuelTypes() const;

    void loadStateFromFile(const std::string& filename);
    void saveStateToFile(const std::string& filename) const;

private:
    FuelTank* findTankForFuel(const std::string& fuelType);
    std::optional<int> findFreeColumnId(const std::string& fuelType);
    
    std::vector<FuelTank> tanks;
    std::vector<FuelColumn> columns;
    std::vector<Transaction> salesHistory;
    std::map<std::string, double> fuelPrices;
};