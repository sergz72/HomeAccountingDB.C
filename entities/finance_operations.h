#ifndef H_FINANCE_OPERATIONS_H
#define H_FINANCE_OPERATIONS_H

#include <map>
#include "common.h"
#include "../json/json_parser.h"
#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
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

class FinOpPropertyParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) const override;
    void parseValue(int field_id) override;
public:
    FinOpProperty property;

    explicit FinOpPropertyParser(JsonParser *p): JsonObjectParser(p) {}
};

class FinOpPropertiesJsonSource: public JsonObjectArrayParser<FinOpProperty> {
    FinOpPropertyParser *parser;
public:
    inline explicit FinOpPropertiesJsonSource(JsonParser *p): JsonObjectArrayParser<FinOpProperty>() {
        parser = new FinOpPropertyParser(p);
    }

    ~FinOpPropertiesJsonSource() override;

protected:
    FinOpProperty *create() override;
    unsigned long getId(const FinOpProperty *value) const override;
    JsonParser *getParser() override;
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

class FinanceOperationParser: public JsonObjectParser {
    FinOpPropertiesJsonSource *propertiesSource;
    long propertiesCapacity;
protected:
    int parseName(std::string &n) const override;
    void parseValue(int field_id) override;
public:
    FinanceOperation operation;

    explicit FinanceOperationParser(const char * file_name, long _propertiesCapacity): JsonObjectParser(file_name) {
        propertiesSource = new FinOpPropertiesJsonSource(parser);
        propertiesCapacity = _propertiesCapacity;
    }
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
