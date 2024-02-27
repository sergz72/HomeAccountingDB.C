#ifndef H_FINANCE_OPERATIONS_H
#define H_FINANCE_OPERATIONS_H

#include <map>
#include "common.h"
#include "subcategories.h"
#include "accounts.h"

#define FIN_OP_PROPERTY_STRING_VALUE_LENGTH 80

enum FinOpPropertyCode {
    amou,
    dist,
    netw,
    ppto,
    seca,
    type
};

struct FinOpProperty {
     long numericValue;
     char stringValue[FIN_OP_PROPERTY_STRING_VALUE_LENGTH];
     unsigned long dateValue;
     FinOpPropertyCode code;
};

class FinOpProperties: public ObjectArray<FinOpProperty> {
public:
    inline explicit FinOpProperties(ObjectArraySource<FinOpProperty> *source, long capacity):
        ObjectArray<FinOpProperty>(source, capacity) {
    }

protected:
    bool isValid(const FinOpProperty *value) const override;
};

struct FinanceOperation {
    unsigned long date;
    long amount;
    long summa;
    unsigned long subcategory;
    unsigned long account;
    FinOpProperties *finOpProperties;
};

struct FinanceChanges {
    long summa;
    long income;
    long expenditure;

    inline long getEndSumma() const {
        return summa + income - expenditure;
    }
};

class FinanceOperations: public ObjectArray<FinanceOperation> {
protected:
    bool isValid(const FinanceOperation *value) const override;
public:
    std::map<unsigned long, long> totals;

    inline explicit FinanceOperations(ObjectArraySource<FinanceOperation> *source, long capacity):
        ObjectArray<FinanceOperation>(source, capacity) {
    }

    void calculateTotals(FinanceOperations *prev, Accounts *accounts, Subcategories *subcategories);
    void buildChanges(std::map<unsigned long, FinanceChanges> &changes, unsigned long from_date, unsigned long to_date,
                      Accounts *accounts, Subcategories *subcategories);
    void printChanges(unsigned long _date, Accounts *accounts, Subcategories *subcategories);
};

#endif
