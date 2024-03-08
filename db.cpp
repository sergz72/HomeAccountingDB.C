#include <iostream>
#include "db.h"

long DB::calculateKey(long date) const {
    date /= 100;
    long month = date % 100;
    long year = date / 100;
    return (month - minMonth) + (year - minYear) * 12;
}

void DB::calculateTotals(unsigned long from) {
    FinanceOperations *prev = nullptr;
    while (from < count) {
        auto data = get(from);
        if (data != nullptr) {
            data->calculateTotals(prev, accounts, subcategories);
            prev = data;
        }
        from++;
    }
}

void DB::printChanges(unsigned long date) {
    unsigned long idx = validateDataIndex((long)date);
    auto data = get(idx);
    if (data == nullptr)
        std::cout << "No data" << std::endl;
    else {
        data->printChanges(date, accounts, subcategories);
    }
}
