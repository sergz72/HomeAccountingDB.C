#ifndef H_JSON_DB_CONFIG_H
#define H_JSON_DB_CONFIG_H

#include "../db_config.h"

class JsonDBConfig: public DBConfiguration {
    const char *base_folder;
    long capacity;
    long propertiesCapacity;
public:
    inline explicit JsonDBConfig(const char *_base_folder, long _capacity, long _propertiesCapacity) {
        base_folder = _base_folder;
        capacity = _capacity;
        propertiesCapacity = _propertiesCapacity;
        propertiesCapacity = _propertiesCapacity;
    }

    ObjectArraySource<Account> *getAccountsSource() override;
    ObjectArraySource<Subcategory> *getSubcategoriesSource() override;
    DatedSource<FinanceOperations> *getMainDataSource() override;
};

#endif
