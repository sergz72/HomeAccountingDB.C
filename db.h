#ifndef HOMEACCOUNTINGDB_DB_H
#define HOMEACCOUNTINGDB_DB_H

#include "core/time_series_data.h"
#include "entities/finance_operations.h"
#include "entities/accounts.h"
#include "entities/subcategories.h"

class DB: public TimeSeriesData<FinanceOperation, FinanceOperations> {
    Accounts *accounts;
    Subcategories *subcategories;
    long operations_capacity;
    long min_month;
    long min_year;
protected:
    FinanceOperations* createObject() override;
    long getDataIndex(long date) override;
public:
    inline DB(const char *base_path, long _min_year, long _min_month, long accounts_capacity,
              long subcategories_capacity, long _operations_capacity, long time_series_data_capacity):
    TimeSeriesData<FinanceOperation, FinanceOperations>(std::string(base_path).append("/dates"), time_series_data_capacity) {
        operations_capacity = _operations_capacity;
        min_year = _min_year;
        min_month = _min_month;
        accounts = new Accounts(base_path, accounts_capacity);
        accounts->parse();
        subcategories = new Subcategories(base_path, subcategories_capacity);
        subcategories->parse();
    }

    inline ~DB() {
        delete accounts;
        delete subcategories;
    }

    void calculateTotals();
};


#endif
