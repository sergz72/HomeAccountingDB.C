#include <iostream>
#include "finance_operations.h"

bool FinOpProperties::isValid(const FinOpProperty *value) const {
    return true;
}

bool FinanceOperations::isValid(const FinanceOperation *value) const {
    return true;
}

static void handleTrfrWithSumma(std::map<unsigned long, FinanceChanges> &changes, FinanceOperation *op, long summa) {
    auto properties = op->finOpProperties;
    if (properties == nullptr || properties->getCount() != 1)
        return;
    auto prop = properties->get(0);
    if (prop->code != seca)
        return;
    changes[op->account].expenditure += summa;
    changes[prop->numericValue].income += op->summa;
}

static void apply(std::map<unsigned long, FinanceChanges> &changes, FinanceOperation *op, Accounts *accounts,
           Subcategories *subcategories) {
    auto subcategory = subcategories->get(op->subcategory);
    switch (subcategory->operationCode) {
        case incm:
            changes[op->account].income += op->summa;
            break;
        case expn:
            changes[op->account].expenditure += op->summa;
            break;
        case spcl:
            switch (subcategory->code) {
                case incc:
                    // Пополнение карточного счета наличными
                    changes[op->account].income += op->summa;
                    changes[accounts->getCashAccount(op->account)].expenditure += op->summa;
                    break;
                case expc:
                    // Снятие наличных в банкомате
                    changes[op->account].expenditure += op->summa;
                    changes[accounts->getCashAccount(op->account)].income += op->summa;
                    break;
                case exch:
                    // Обмен валюты
                    handleTrfrWithSumma(changes, op, op->amount / 10);
                    break;
                case trfr:
                    // Перевод средств между платежными картами
                    handleTrfrWithSumma(changes, op, op->summa);
                    break;
                default:
                    throw std::runtime_error("invalid subcategory code");
            }
            break;
    }
}

void initChanges(std::map<unsigned long, FinanceChanges> &changes, std::map<unsigned long, long> &totals) {
    changes.clear();
    for (auto kv : totals) {
        changes.insert({kv.first, FinanceChanges(kv.second, 0, 0)});
    }
}

void initTotals(std::map<unsigned long, long> &totals, std::map<unsigned long, FinanceChanges> &changes) {
    totals.clear();
    for (auto kv : changes) {
        totals.insert({kv.first, kv.second.getEndSumma()});
    }
}

void FinanceOperations::calculateTotals(FinanceOperations *prev, Accounts *accounts, Subcategories *subcategories) {
    if (prev != nullptr) {
        std::map<unsigned long, FinanceChanges> changes;
        initChanges(changes, prev->totals);
        prev->buildChanges(changes, 1, 99999999, accounts, subcategories);
        initTotals(totals, changes);
    }
}

void FinanceOperations::buildChanges(std::map<unsigned long, FinanceChanges> &changes, unsigned long from_date,
                                     unsigned long to_date, Accounts *accounts, Subcategories *subcategories) {
    FinanceOperation *op = array;
    for (long i = 0; i < count; i++) {
        if (op->date >= from_date && op->date <= to_date)
            apply(changes, op, accounts, subcategories);
        op++;
    }
}

void FinanceOperations::printChanges(unsigned long _date, Accounts *accounts, Subcategories *subcategories) {
    std::map<unsigned long, FinanceChanges> changes;
    initChanges(changes, totals);
    buildChanges(changes, 1, _date - 1, accounts, subcategories);
    std::map<unsigned long, long> day_totals;
    initTotals(day_totals, changes);
    initChanges(changes, day_totals);
    buildChanges(changes, _date, _date, accounts, subcategories);
    for (auto kv : changes) {
        auto account = accounts->get(kv.first);
        std::cout << account->name << " " << kv.second.summa << " " << kv.second.income << " " << kv.second.expenditure
            << " " << kv.second.getEndSumma() << std::endl;
    }
}
