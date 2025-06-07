#pragma once
#include <string>
#include <chrono>

struct Transaction {
    std::chrono::system_clock::time_point timestamp;
    int columnId;
    std::string fuelType;
    double amountLiters;
    double totalPrice;
};