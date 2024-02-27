#ifndef HOMEACCOUNTINGDB_DB_H
#define HOMEACCOUNTINGDB_DB_H

#include "db_config.h"

class DB: public TimeSeriesData<FinanceOperations> {
    Accounts *accounts;
    Subcategories *subcategories;
    unsigned long operations_capacity;
    long min_month;
    long min_year;
protected:
    long calculateKey(long date) const override;
public:
    inline DB(const char *base_path, long _min_year, long _min_month, unsigned long accounts_capacity,
              unsigned long subcategories_capacity, unsigned long _operations_capacity, unsigned long time_series_data_capacity,
              DBConfiguration *configuration):
              TimeSeriesData<FinanceOperations>(std::string(base_path).append("/dates"),
                                                configuration->getMainDataSource(), time_series_data_capacity) {
        operations_capacity = _operations_capacity;
        min_year = _min_year;
        min_month = _min_month;
        accounts = new Accounts(configuration->getAccountsSource(), accounts_capacity);
        subcategories = new Subcategories(configuration->getSubcategoriesSource(), subcategories_capacity);
    }

    inline ~DB() {
        delete accounts;
        delete subcategories;
    }

    void calculateTotals(unsigned long from);

    void printChanges(unsigned long date);
};

#endif
