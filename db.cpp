#include <iostream>
#include "db.h"

long DB::getDataIndex(long date) {
    date /= 100;
    long month = date % 100;
    long year = date / 100;
    return (month - min_month) + (year - min_year) * 12;
}

FinanceOperations *DB::createObject() {
    return new FinanceOperations(operations_capacity);
}

void DB::calculateTotals(long from) {
    FinanceOperations *prev = nullptr;
    while (from < count) {
        auto ops = data[from];
        if (ops != nullptr) {
            ops->calculateTotals(prev, accounts, subcategories);
            prev = ops;
        }
        from++;
    }
}

void DB::printChanges(long date) {
    long idx = validateDataIndex(date);
    auto ops = data[idx];
    if (ops == nullptr)
        std::cout << "No data" << std::endl;
    else {
        ops->printChanges(date, accounts, subcategories);
    }
}
