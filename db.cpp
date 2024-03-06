#include <iostream>
#include "db.h"

long DB::calculateKey(long date) const {
    date /= 100;
    long month = date % 100;
    long year = date / 100;
    return (month - min_month) + (year - min_year) * 12;
}

void DB::calculateTotals(unsigned long from) {
    FinanceOperations *prev = nullptr;
    while (from < count) {
        auto ops = data[from];
        if (ops != nullptr && ops->data != nullptr) {
            ops->data->calculateTotals(prev, accounts, subcategories);
            prev = ops->data;
        }
        from++;
    }
}

void DB::printChanges(unsigned long date) {
    unsigned long idx = validateDataIndex((long)date);
    auto ops = data[idx];
    if (ops == nullptr || ops->data == nullptr)
        std::cout << "No data" << std::endl;
    else {
        ops->data->printChanges(date, accounts, subcategories);
    }
}
