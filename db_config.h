#ifndef H_DB_CONFIG_H
#define H_DB_CONFIG_H

#include "core/time_series_data.h"
#include "entities/finance_operations.h"
#include "entities/accounts.h"
#include "entities/subcategories.h"

class DBConfiguration {
public:
    virtual ObjectArraySource<Account> *getAccountsSource() = 0;
    virtual ObjectArraySource<Subcategory> *getSubcategoriesSource() = 0;
    virtual DatedSource<FinanceOperations> *getMainDataSource() = 0;
};

#endif
