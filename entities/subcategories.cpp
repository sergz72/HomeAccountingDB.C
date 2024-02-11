#include "subcategories.h"

#define SUBCATEGORY_ID 1
#define SUBCATEGORY_NAME 2
#define SUBCATEGORY_CATEGORY 3
#define SUBCATEGORY_CODE 4
#define SUBCATEGORY_OPERATION_CODE 5

int SubcategoryParser::parseName(std::string *n) {
    if (*n == "id")
        return SUBCATEGORY_ID;
    else if (*n == "name")
        return SUBCATEGORY_NAME;
    else if (*n == "categoryId")
        return SUBCATEGORY_CATEGORY;
    else if (*n == "code")
        return SUBCATEGORY_CODE;
    else if (*n == "operationCodeId")
        return SUBCATEGORY_OPERATION_CODE;

    return 0;
}

void SubcategoryParser::parseValue(int field_id) {
    switch (field_id) {
        case SUBCATEGORY_ID:
            subcategory.id = parser->expectedInteger();
            break;
        case SUBCATEGORY_NAME:
            parser->expectedString();
            strncpy(subcategory.name, parser->current.string_value.c_str(), SUBCATEGORY_NAME_SIZE);
            break;
        case SUBCATEGORY_CATEGORY:
            subcategory.category = parser->expectedInteger();
            break;
        case SUBCATEGORY_OPERATION_CODE:
            parser->expectedString();
            if (parser->current.string_value.length() != 4)
                throw std::runtime_error("incorrect subcategory operation code");
            subcategory.operationCode.int_value = 0;
            strcpy(subcategory.operationCode.char_value, parser->current.string_value.c_str());
            break;
        case SUBCATEGORY_CODE:
            subcategory.code.int_value = 0;
            parser->nextToken();
            if (parser->current.isNull()) {
                break;
            }
            parser->isString();
            if (parser->current.string_value.length() != 4)
                throw std::runtime_error("incorrect subcategory code");
            strcpy(subcategory.code.char_value, parser->current.string_value.c_str());
            break;
        default:
            break;
    }
}

Subcategories::Subcategories(const char *data_folder, long capacity): JsonObjectArrayParser<Subcategory>(capacity) {
    auto file_name = std::string(data_folder);
    file_name += "/subcategories.json";
    parser = new SubcategoryParser(file_name.c_str());
}

Subcategory *Subcategories::create(JsonParser *p) {
    parser->parse(false);
    return &parser->subcategory;
}

long Subcategories::getId(Subcategory *value) {
    return value->id;
}

bool Subcategories::isValid(Subcategory *value) {
    return value->id > 0;
}
