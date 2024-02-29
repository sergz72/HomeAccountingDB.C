#include "finance_operations.h"
#include <cstring>

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

int FinOpPropertyParser::parseName(std::string &n) const {
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

FinOpPropertiesJsonSource::~FinOpPropertiesJsonSource() {
    delete parser;
}

FinOpProperty *FinOpPropertiesJsonSource::create() {
    parser->parse(false);
    return &parser->property;
}

unsigned long FinOpPropertiesJsonSource::getId(const FinOpProperty *value) const {
    return 0;
}

JsonParser *FinOpPropertiesJsonSource::getParser() {
    return parser->parser;
}

int FinanceOperationParser::parseName(std::string &n) const {
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
    long id;
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
            id = parser->expectedInteger();
            if (id <= 0)
                throw std::runtime_error("invalid account id");
            operation.account = id;
            break;
        case FINOP_SUBCATEGORY:
            id = parser->expectedInteger();
            if (id <= 0)
                throw std::runtime_error("invalid subcategory id");
            operation.subcategory = id;
            break;
        case FINOP_PROPERTIES:
            parser->nextToken();
            if (parser->current.isNull())
                operation.finOpProperties = nullptr;
            else {
                operation.finOpProperties = new FinOpProperties(propertiesSource, propertiesCapacity);
            }
            break;
        case FINOP_ID:
            parser->expectedInteger();
            break;
        default:
            break;
    }
}
