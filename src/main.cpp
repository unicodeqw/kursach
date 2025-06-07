#include "GasStation.h"
#include <iostream>
#include <limits>
#include <string>

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printMenu(const std::vector<std::string>& fuelTypes) {
    std::cout << "\n========= Меню Автозаправної Станції =========\n";
    std::cout << "1. Заправити автомобіль (за літрами)\n";
    std::cout << "2. Заправити автомобіль (на суму)\n";
    std::cout << "3. Показати стан резервуарів\n";
    std::cout << "4. Показати звіт про продажі\n";
    std::cout << "5. Вийти\n";
    std::cout << "----------------------------------------------\n";
    std::cout << "Доступні типи пального: ";
    for (size_t i = 0; i < fuelTypes.size(); ++i) {
        std::cout << fuelTypes[i] << (i == fuelTypes.size() - 1 ? "" : ", ");
    }
    std::cout << "\nВаш вибір: ";
}

int main() {
    // Встановлення української локалі для коректного виводу
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    GasStation station;
    const std::string stateFile = "data/station_state.txt";
    station.loadStateFromFile(stateFile);

    int choice = 0;
    while (choice != 5) {
        printMenu(station.getAvailableFuelTypes());
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Помилка вводу. Будь ласка, введіть число.\n";
            clearInputBuffer();
            choice = 0; // Скидаємо вибір, щоб уникнути зациклення
            continue;
        }

        switch (choice) {
            case 1: {
                std::string type;
                double liters;
                std::cout << "Введіть тип пального: ";
                std::cin >> type;
                std::cout << "Введіть кількість літрів: ";
                std::cin >> liters;
                if(std::cin.fail()) {
                    std::cout << "Некоректна кількість літрів.\n";
                    clearInputBuffer();
                    break;
                }
                station.serveCustomerByLiters(type, liters);
                break;
            }
            case 2: {
                std::string type;
                double sum;
                std::cout << "Введіть тип пального: ";
                std::cin >> type;
                std::cout << "Введіть суму (грн): ";
                std::cin >> sum;
                if(std::cin.fail()) {
                    std::cout << "Некоректна сума.\n";
                    clearInputBuffer();
                    break;
                }
                station.serveCustomerBySum(type, sum);
                break;
            }
            case 3: {
                station.printTankStatus();
                break;
            }
            case 4: {
                station.printDailyReport();
                break;
            }
            case 5: {
                std::cout << "Завершення роботи. Збереження стану...\n";
                break;
            }
            default: {
                std::cout << "Невірний вибір. Спробуйте ще раз.\n";
                break;
            }
        }
    }

    station.saveStateToFile(stateFile);

    return 0;
}