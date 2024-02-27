#include "json_db_config.h"
#include "accounts.h"

ObjectArraySource<Account> *JsonDBConfig::getAccountsSource() {
    return new AccountsJsonSource()
}

ObjectArraySource<Subcategory> *JsonDBConfig::getSubcategoriesSource() {

}

DatedSource<FinanceOperations> *JsonDBConfig::getMainDataSource() {

}
