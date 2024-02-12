#include <iostream>
#include "finance_operations.h"

#define PROPERTY_NUMERIC_VALUE 1
#define PROPERTY_STRING_VALUE 2
#define PROPERTY_DATE_VALUE 3
#define PROPERTY_CODE 4

#define FINOP_ID 1
#define FINOP_AMOUNT 2
#define FINOP_SUMMA 3
#define FINOP_ACCOUNT 4
#define FINOP_SUBCATEGORY 5
#define FINOP_PROPERTIES 6

int FinOpPropertyParser::parseName(std::string &n) {
    if (n == "numericValue" || n == "NumericValue")
        return PROPERTY_NUMERIC_VALUE;
    if (n == "stringValue" || n == "StringValue")
        return PROPERTY_STRING_VALUE;
    if (n == "dateValue" || n == "DateValue")
        return PROPERTY_DATE_VALUE;
    if (n == "propertyCode" || n == "PropertyCode")
        return PROPERTY_CODE;

    return 0;
}

void FinOpPropertyParser::parseValue(int field_id) {
    switch (field_id) {
        case PROPERTY_NUMERIC_VALUE:
            parser->nextToken();
            if (parser->current.isNull())
                property.numericValue = 0;
            else {
                property.numericValue = parser->isInteger();
            }
            break;
        case PROPERTY_STRING_VALUE:
            parser->nextToken();
            if (parser->current.isNull())
                property.stringValue[0] = 0;
            else {
                parser->isString();
                strcpy(property.stringValue, parser->current.string_value.c_str());
            }
            break;
        case PROPERTY_DATE_VALUE:
            property.dateValue = parse_date(parser);
            break;
        case PROPERTY_CODE:
            parser->expectedString();
            if (parser->current.string_value == "AMOU")
                property.code = amou;
            else if (parser->current.string_value == "DIST")
                property.code = dist;
            else if (parser->current.string_value == "NETW")
                property.code = netw;
            else if (parser->current.string_value == "PPTO")
                property.code = ppto;
            else if (parser->current.string_value == "SECA")
                property.code = seca;
            else if (parser->current.string_value == "TYPE")
                property.code = type;
            else
                throw std::runtime_error("incorrect property code");
            break;
        default:
            break;
    }
}

FinOpProperty *FinOpProperties::create(JsonParser *p) {
    parser->parse(false);
    return &parser->property;
}

long FinOpProperties::getId(FinOpProperty *value) {
    return -1;
}

bool FinOpProperties::isValid(FinOpProperty *value) {
    return true;
}

int FinanceOperationParser::parseName(std::string &n) {
    if (n == "id" || n == "Id")
        return FINOP_ID;
    if (n == "amount" || n == "Amount")
        return FINOP_AMOUNT;
    if (n == "summa" || n == "Summa")
        return FINOP_SUMMA;
    if (n == "subcategoryId" || n == "SubcategoryId")
        return FINOP_SUBCATEGORY;
    if (n == "accountId" || n == "AccountId")
        return FINOP_ACCOUNT;
    if (n == "finOpProperies" || n == "FinOpProperies")
        return FINOP_PROPERTIES;

    return 0;
}

void FinanceOperationParser::parseValue(int field_id) {
    JsonTokenType typ;
    switch (field_id) {
        case FINOP_AMOUNT:
            parser->nextToken();
            if (parser->current.isNull())
                operation.amount = 0;
            else {
                typ = parser->current.typ;
                if (typ != integer && typ != real)
                    throw std::runtime_error("integer or double is expected");
                operation.amount = parser->current.int_value;
            }
            break;
        case FINOP_SUMMA:
            parser->nextToken();
            typ = parser->current.typ;
            if (typ != integer && typ != real)
                throw std::runtime_error("integer or double is expected");
            operation.summa = parser->current.int_value;
            break;
        case FINOP_ACCOUNT:
            operation.account = parser->expectedInteger();
            break;
        case FINOP_SUBCATEGORY:
            operation.subcategory = parser->expectedInteger();
            break;
        case FINOP_PROPERTIES:
            parser->nextToken();
            if (parser->current.isNull())
                operation.finOpProperties = nullptr;
            else {
                operation.finOpProperties = new FinOpProperties(parser, FIN_OP_PROPERTIES_CAPACITY);
                operation.finOpProperties->parse();
            }
            break;
        case FINOP_ID:
            parser->expectedInteger();
            break;
        default:
            break;
    }
}

FinanceOperation *FinanceOperations::create(JsonParser *p) {
    parser->parse(false);
    parser->operation.date = date;
    return &parser->operation;
}

long FinanceOperations::getId(FinanceOperation *value) {
    return -1;
}

bool FinanceOperations::isValid(FinanceOperation *value) {
    return true;
}

static void handleTrfrWithSumma(std::map<long, FinanceChanges> &changes, FinanceOperation *op, long summa) {
    auto properties = op->finOpProperties;
    if (properties == nullptr || properties->getCount() != 1)
        return;
    auto prop = properties->get(0);
    if (prop->code != seca)
        return;
    changes[op->account].expenditure += summa;
    changes[prop->numericValue].income += op->summa;
}

static void apply(std::map<long, FinanceChanges> &changes, FinanceOperation *op, Accounts *accounts,
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

void initChanges(std::map<long, FinanceChanges> &changes, std::map<long, long> &totals) {
    changes.clear();
    for (auto kv : totals) {
        changes.insert({kv.first, FinanceChanges(kv.second, 0, 0)});
    }
}

void initTotals(std::map<long, long> &totals, std::map<long, FinanceChanges> &changes) {
    totals.clear();
    for (auto kv : changes) {
        totals.insert({kv.first, kv.second.getEndSumma()});
    }
}

void FinanceOperations::calculateTotals(FinanceOperations *prev, Accounts *accounts, Subcategories *subcategories) {
    if (prev != nullptr) {
        std::map<long, FinanceChanges> changes;
        initChanges(changes, prev->totals);
        prev->buildChanges(changes, 1, 99999999, accounts, subcategories);
        initTotals(totals, changes);
    }
}

void FinanceOperations::buildChanges(std::map<long, FinanceChanges> &changes, long from_date, long to_date,
                                     Accounts *accounts, Subcategories *subcategories) {
    FinanceOperation *op = array;
    for (long i = 0; i < count; i++) {
        if (op->date >= from_date && op->date <= to_date)
            apply(changes, op, accounts, subcategories);
        op++;
    }
}

void FinanceOperations::printChanges(long _date, Accounts *accounts, Subcategories *subcategories) {
    std::map<long, FinanceChanges> changes;
    initChanges(changes, totals);
    buildChanges(changes, 1, _date - 1, accounts, subcategories);
    std::map<long, long> day_totals;
    initTotals(day_totals, changes);
    initChanges(changes, day_totals);
    buildChanges(changes, _date, _date, accounts, subcategories);
    for (auto kv : changes) {
        auto account = accounts->get(kv.first);
        std::cout << account->name << " " << kv.second.summa << " " << kv.second.income << " " << kv.second.expenditure
            << " " << kv.second.getEndSumma() << std::endl;
    }
}
