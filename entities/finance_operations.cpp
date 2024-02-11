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

int FinOpPropertyParser::parseName(std::string *n) {
    if (*n == "numericValue" || *n == "NumericValue")
        return PROPERTY_NUMERIC_VALUE;
    if (*n == "stringValue" || *n == "StringValue")
        return PROPERTY_STRING_VALUE;
    if (*n == "dateValue" || *n == "DateValue")
        return PROPERTY_DATE_VALUE;
    if (*n == "propertyCode" || *n == "PropertyCode")
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
            if (parser->current.string_value.length() != 4)
                throw std::runtime_error("incorrect property code");
            strcpy(property.code.char_value, parser->current.string_value.c_str());
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

int FinanceOperationParser::parseName(std::string *n) {
    if (*n == "id" || *n == "Id")
        return FINOP_ID;
    if (*n == "amount" || *n == "Amount")
        return FINOP_AMOUNT;
    if (*n == "summa" || *n == "Summa")
        return FINOP_SUMMA;
    if (*n == "subcategoryId" || *n == "SubcategoryId")
        return FINOP_SUBCATEGORY;
    if (*n == "accountId" || *n == "AccountId")
        return FINOP_ACCOUNT;
    if (*n == "finOpProperies" || *n == "FinOpProperies")
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
        default:
            break;
    }
}

FinanceOperation *FinanceOperations::create(JsonParser *p) {
    parser->parse(false);
    return &parser->operation;
}

long FinanceOperations::getId(FinanceOperation *value) {
    return -1;
}

bool FinanceOperations::isValid(FinanceOperation *value) {
    return true;
}
