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
#define FIN_OP_PROPERTIES_CAPACITY 5

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
     long dateValue;
     FinOpPropertyCode code;
};

class FinOpPropertyParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) override;
    void parseValue(int field_id) override;
public:
    FinOpProperty property;

    explicit FinOpPropertyParser(JsonParser *p): JsonObjectParser(p) {}
};

class FinOpProperties: public JsonObjectArrayParser<FinOpProperty> {
    FinOpPropertyParser *parser;
public:
    inline explicit FinOpProperties(JsonParser *p, long capacity): JsonObjectArrayParser<FinOpProperty>(capacity) {
        parser = new FinOpPropertyParser(p);
    }

    inline ~FinOpProperties() {
        delete parser;
    }

    inline void parse() {
        parse_array(parser->parser, false);
    }

protected:
    FinOpProperty *create(JsonParser *p) override;
    long getId(FinOpProperty *value) override;
    bool isValid(FinOpProperty *value) override;
};

struct FinanceOperation {
    long date;
    long amount;
    long summa;
    long subcategory;
    long account;
    FinOpProperties *finOpProperties;
};

class FinanceOperationParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) override;
    void parseValue(int field_id) override;
public:
    FinanceOperation operation;

    explicit FinanceOperationParser(const char * file_name): JsonObjectParser(file_name) {}
};

struct FinanceChanges {
    long summa;
    long income;
    long expenditure;

    inline long getEndSumma() const {
        return summa + income - expenditure;
    }
};

class FinanceOperations: public JsonObjectArrayParser<FinanceOperation> {
    FinanceOperationParser *parser;
    long date;
protected:
    FinanceOperation *create(JsonParser *p) override;
    long getId(FinanceOperation *value) override;
    bool isValid(FinanceOperation *value) override;
public:
    std::map<long, long> totals;

    inline explicit FinanceOperations(long capacity): JsonObjectArrayParser<FinanceOperation>(capacity) {
        parser = nullptr;
    }

    inline void parse(const char *file_name, long _date) {
        date = _date;
        parser = new FinanceOperationParser(file_name);
        parse_array(parser->parser);
        delete parser;
    }

    void calculateTotals(FinanceOperations *prev, Accounts *accounts, Subcategories *subcategories);
    void buildChanges(std::map<long, FinanceChanges> &changes, long from_date, long to_date, Accounts *accounts,
                      Subcategories *subcategories);
    void printChanges(long _date, Accounts *accounts, Subcategories *subcategories);
};

#endif
