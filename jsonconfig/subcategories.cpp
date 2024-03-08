#include "subcategories.h"
#include <cstring>

#define SUBCATEGORY_ID 1
#define SUBCATEGORY_NAME 2
#define SUBCATEGORY_CATEGORY 3
#define SUBCATEGORY_CODE 4
#define SUBCATEGORY_OPERATION_CODE 5

int SubcategoryParser::parseName(std::string &n) const {
    if (n == "id")
        return SUBCATEGORY_ID;
    else if (n == "name")
        return SUBCATEGORY_NAME;
    else if (n == "categoryId")
        return SUBCATEGORY_CATEGORY;
    else if (n == "code")
        return SUBCATEGORY_CODE;
    else if (n == "operationCodeId")
        return SUBCATEGORY_OPERATION_CODE;

    return 0;
}

void SubcategoryParser::parseValue(int fieldId) {
    switch (fieldId) {
        case SUBCATEGORY_ID:
            subcategory.id = parser->expectedInteger();
            break;
        case SUBCATEGORY_NAME:
            parser->expectedString();
            strncpy(subcategory.name, parser->current.stringValue.c_str(), SUBCATEGORY_NAME_SIZE);
            break;
        case SUBCATEGORY_CATEGORY:
            subcategory.category = parser->expectedInteger();
            break;
        case SUBCATEGORY_OPERATION_CODE:
            parser->expectedString();
            if (parser->current.stringValue == "INCM")
                subcategory.operationCode = incm;
            else if (parser->current.stringValue == "EXPN")
                subcategory.operationCode = expn;
            else if (parser->current.stringValue == "SPCL")
                subcategory.operationCode = spcl;
            else
                throw std::runtime_error("incorrect subcategory operation code");
            break;
        case SUBCATEGORY_CODE:
            parser->nextToken();
            if (parser->current.isNull()) {
                subcategory.code = none;
                break;
            }
            parser->isString();
            if (parser->current.stringValue == "INCC")
                subcategory.code = incc;
            else if (parser->current.stringValue == "EXPC")
                subcategory.code = expc;
            else if (parser->current.stringValue == "EXCH")
                subcategory.code = exch;
            else if (parser->current.stringValue == "TRFR")
                subcategory.code = trfr;
            else if (parser->current.stringValue == "COMB")
                subcategory.code = comb;
            else if (parser->current.stringValue == "COMC")
                subcategory.code = comc;
            else if (parser->current.stringValue == "FUEL")
                subcategory.code = fuel;
            else if (parser->current.stringValue == "PRCN")
                subcategory.code = prcn;
            else
                throw std::runtime_error("incorrect subcategory code");
            break;
        default:
            break;
    }
}

SubcategoriesJsonSource::SubcategoriesJsonSource(const char *dataFolder) : JsonObjectArrayParser() {
    auto file_name = std::string(dataFolder);
    file_name += "/subcategories.json";
    parser = new SubcategoryParser(file_name.c_str());
}

SubcategoriesJsonSource::~SubcategoriesJsonSource() {
    delete parser;
}

Subcategory *SubcategoriesJsonSource::create() {
    parser->parse(false);
    return &parser->subcategory;
}

unsigned long SubcategoriesJsonSource::getId(const Subcategory *value) const {
    return value->id;
}

JsonParser* SubcategoriesJsonSource::getParser() {
    return parser->parser;
}
