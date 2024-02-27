#ifndef H_JSON_DB_CONFIG_H
#define H_JSON_DB_CONFIG_H

#include "../db_config.h"

class JsonDBConfig: public DBConfiguration {
    ObjectArraySource<Account> *getAccountsSource() override;
    ObjectArraySource<Subcategory> *getSubcategoriesSource() override;
    DatedSource<FinanceOperations> *getMainDataSource() override;
};

#endif
