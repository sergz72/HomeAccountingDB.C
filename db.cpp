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

void DB::calculateTotals() {

}
