#ifndef HOMEACCOUNTINGDB_DB_H
#define HOMEACCOUNTINGDB_DB_H

#include "db_config.h"

class DB: public TimeSeriesData<FinanceOperations> {
    Accounts *accounts;
    Subcategories *subcategories;
    long minMonth;
    long minYear;
protected:
    long calculateKey(long date) const override;
public:
    inline DB(const char *basePath, long _minYear, long _minMonth, unsigned long accountsCapacity,
              unsigned long subcategoriesCapacity, unsigned long timeSeriesDataCapacity,
              long maxTimeSeriesActiveItems, DBConfiguration *configuration):
              TimeSeriesData(std::string(basePath).append("/dates"),
                  configuration->getMainDataSource(), timeSeriesDataCapacity, maxTimeSeriesActiveItems) {
        minYear = _minYear;
        minMonth = _minMonth;
        accounts = new Accounts(configuration->getAccountsSource(), accountsCapacity);
        subcategories = new Subcategories(configuration->getSubcategoriesSource(), subcategoriesCapacity);
    }

    inline ~DB() {
        delete accounts;
        delete subcategories;
    }

    void calculateTotals(unsigned long from);

    void printChanges(unsigned long date);
};

#endif
