#include "accounts.h"
#include <map>

bool Accounts::isValid(const Account *value) const {
    return value->id > 0;
}

void Accounts::buildCashAccounts() {
    std::map<long, long> cash_accounts;
    auto el = array;
    for (long i = 0; i < count; i++) {
        if (isValid(el)) {
            if (el->cashAccount == 0) {
                if (cash_accounts.contains(el->currency.intValue))
                    throw std::runtime_error("duplicate cash account");
                cash_accounts.insert({el->currency.intValue, el->id});
            }
        }
        el++;
    }

    el = array;
    for (long i = 0; i < count; i++) {
        if (isValid(el)) {
            if (el->cashAccount == 1) {
                if (!cash_accounts.contains(el->currency.intValue))
                    throw std::runtime_error("cash account not found");
                el->cashAccount = cash_accounts[el->currency.intValue];
            }
        }
        el++;
    }
}

unsigned long Accounts::getCashAccount(unsigned long id) {
    auto account = get(id);
    auto acc = account->cashAccount;
    if (acc < 0)
        throw std::runtime_error("invalid cash account");
    return acc;
}
