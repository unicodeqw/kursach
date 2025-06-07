#include "GasStation.h"
#include <iostream>
#include <format>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>

GasStation::GasStation() {
    // Ініціалізація цін
    fuelPrices["A-95"] = 55.49;
    fuelPrices["Diesel"] = 52.99;
    fuelPrices["LPG"] = 26.79;

    // Ініціалізація резервуарів
    tanks.emplace_back("A-95", 10000.0, 7500.0);
    tanks.emplace_back("Diesel", 8000.0, 6000.0);
    tanks.emplace_back("LPG", 5000.0, 4000.0);
    
    // Ініціалізація колонок і прив'язка їх до резервуарів
    columns.emplace_back(1, findTankForFuel("A-95"));
    columns.emplace_back(2, findTankForFuel("A-95"));
    columns.emplace_back(3, findTankForFuel("Diesel"));
    columns.emplace_back(4, findTankForFuel("LPG"));
}

FuelTank* GasStation::findTankForFuel(const std::string& fuelType) {
    auto it = std::ranges::find_if(tanks, [&](const FuelTank& tank){
        return tank.getFuelType() == fuelType;
    });
    return (it != tanks.end()) ? &(*it) : nullptr;
}

std::optional<int> GasStation::findFreeColumnId(const std::string& fuelType) {
    auto it = std::ranges::find_if(columns, [&](const FuelColumn& col){
        return !col.isBusy() && col.getFuelType() == fuelType;
    });

    if (it != columns.end()) {
        return it->getId();
    }
    return std::nullopt;
}

bool GasStation::serveCustomerByLiters(const std::string& fuelType, double liters) {
    if (!fuelPrices.contains(fuelType)) {
        std::cerr << std::format("Помилка: пальне типу '{}' не доступне.\n", fuelType);
        return false;
    }
    if (liters <= 0) {
        std::cerr << "Помилка: кількість літрів має бути додатною.\n";
        return false;
    }

    auto columnIdOpt = findFreeColumnId(fuelType);
    if (!columnIdOpt) {
        std::cout << std::format("На жаль, усі колонки з пальним '{}' зайняті або відсутні.\n", fuelType);
        return false;
    }
    int columnId = *columnIdOpt;
    auto& column = columns[columnId - 1]; // ID-1 = index

    column.startService();
    std::cout << std::format("Колонка #{} починає заправку...\n", columnId);

    if (column.dispenseFuel(liters)) {
        double pricePerLiter = fuelPrices.at(fuelType);
        double total = liters * pricePerLiter;
        
        salesHistory.emplace_back(
            std::chrono::system_clock::now(),
            columnId,
            fuelType,
            liters,
            total
        );
        
        std::cout << std::format("Заправлено {:.2f} л. До сплати: {:.2f} грн.\n", liters, total);
        column.endService();
        return true;
    } else {
        std::cerr << std::format("Помилка на колонці #{}: недостатньо пального в резервуарі.\n", columnId);
        column.endService();
        return false;
    }
}

bool GasStation::serveCustomerBySum(const std::string& fuelType, double sum) {
    if (!fuelPrices.contains(fuelType)) {
        std::cerr << std::format("Помилка: пальне типу '{}' не доступне.\n", fuelType);
        return false;
    }
    if (sum <= 0) {
        std::cerr << "Помилка: сума має бути додатною.\n";
        return false;
    }
    double pricePerLiter = fuelPrices.at(fuelType);
    double liters = sum / pricePerLiter;
    return serveCustomerByLiters(fuelType, liters);
}

void GasStation::printTankStatus() const {
    std::cout << "\n--- Стан резервуарів ---\n";
    std::cout << std::format("{:<10} | {:>12} | {:>12} | {:>5}\n", "Тип", "Поточний об'єм", "Макс. об'єм", "Заповненість");
    std::cout << "--------------------------------------------------\n";
    for (const auto& tank : tanks) {
        std::cout << std::format("{:<10} | {:>10.2f} л | {:>10.2f} л | {:>4.1f}%\n",
            tank.getFuelType(), tank.getVolume(), tank.getMaxVolume(), tank.getVolumePercentage());
    }
    std::cout << "--------------------------------------------------\n";
}

void GasStation::printDailyReport() const {
    std::cout << "\n--- Звіт про продажі ---\n";
    if (salesHistory.empty()) {
        std::cout << "Сьогодні ще не було продажів.\n";
        return;
    }

    double totalRevenue = 0.0;
    std::map<std::string, double> litersSold;

    std::cout << std::format("{:<22} | {:>4} | {:<8} | {:>10} | {:>12}\n", "Час", "Кол.", "Тип", "Літри", "Сума (грн)");
    std::cout << "-------------------------------------------------------------------\n";

    for (const auto& sale : salesHistory) {
        totalRevenue += sale.totalPrice;
        litersSold[sale.fuelType] += sale.amountLiters;
        
        // C++20 formatting for time
        auto timeStr = std::format("{:%Y-%m-%d %H:%M:%S}", sale.timestamp);
        std::cout << std::format("{:<22} | {:>4} | {:<8} | {:>10.2f} | {:>12.2f}\n",
            timeStr, sale.columnId, sale.fuelType, sale.amountLiters, sale.totalPrice);
    }
    std::cout << "-------------------------------------------------------------------\n";
    std::cout << "Загальний підсумок:\n";
    for (const auto& [type, liters] : litersSold) {
        std::cout << std::format("- Продано {} : {:.2f} л\n", type, liters);
    }
    std::cout << std::format("Загальний дохід: {:.2f} грн\n", totalRevenue);
}

void GasStation::loadStateFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Info: Файл стану не знайдено. Використовуються значення за замовчуванням.\n";
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string fuelType;
        double volume;
        if (ss >> fuelType >> volume) {
            FuelTank* tank = findTankForFuel(fuelType);
            if (tank) {
                // Скидаємо поточний об'єм і додаємо завантажений
                double maxVol = tank->getMaxVolume();
                *tank = FuelTank(fuelType, maxVol, 0); // "Reset"
                tank->addFuel(volume);
            }
        }
    }
    std::cout << "Стан резервуарів успішно завантажено.\n";
}

void GasStation::saveStateToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Помилка: не вдалося відкрити файл для збереження стану.\n";
        return;
    }

    for (const auto& tank : tanks) {
        file << tank.getFuelType() << " " << tank.getVolume() << "\n";
    }
    std::cout << "Стан резервуарів збережено.\n";
}

std::vector<std::string> GasStation::getAvailableFuelTypes() const {
    std::vector<std::string> types;
    for(const auto& [type, price] : fuelPrices) {
        types.push_back(type);
    }
    return types;
}