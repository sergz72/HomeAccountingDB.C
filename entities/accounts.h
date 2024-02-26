#ifndef H_ACCOUNTS_H
#define H_ACCOUNTS_H

#include "../core/object_array.h"
#include "common.h"

#define ACCOUNT_NAME_SIZE 80

struct Account {
    unsigned long id;
    char name[ACCOUNT_NAME_SIZE];
    CharInt currency;
    unsigned long active_to;
    unsigned long cashAccount;
};

class Accounts: public ObjectArray<Account> {
public:
    inline explicit Accounts(ObjectArraySource<Account> *source, unsigned long capacity):
        ObjectArray<Account>(source, capacity) {
        buildCashAccounts();
    }

    void buildCashAccounts();

    unsigned long getCashAccount(unsigned long id);
protected:
    bool isValid(const Account *value) const override;
};

#endif
