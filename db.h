#ifndef HOMEACCOUNTINGDB_DB_H
#define HOMEACCOUNTINGDB_DB_H

#include "core/time_series_data.h"
#include "entities/finance_operations.h"
#include "entities/accounts.h"
#include "entities/subcategories.h"

class DBConfiguration {
public:
    virtual ObjectArraySource<Account> *getAccountsSource() = 0;
    virtual ObjectArraySource<Subcategory> *getSubcategoriesSource() = 0;
};

class DB: public TimeSeriesData<FinanceOperation, FinanceOperations> {
    Accounts *accounts;
    Subcategories *subcategories;
    unsigned long operations_capacity;
    unsigned long min_month;
    unsigned long min_year;
protected:
    FinanceOperations* createObject() override;
    long getDataIndex(long date) override;
public:
    inline DB(const char *base_path, unsigned long _min_year, unsigned long _min_month, unsigned long accounts_capacity,
              unsigned long subcategories_capacity, unsigned long _operations_capacity, unsigned long time_series_data_capacity,
              DBConfiguration *configuration):
    TimeSeriesData<FinanceOperation, FinanceOperations>(std::string(base_path).append("/dates"), time_series_data_capacity) {
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
