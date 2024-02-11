#ifndef H_SUBCATEGORIES_H
#define H_SUBCATEGORIES_H

#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "common.h"

#define SUBCATEGORY_NAME_SIZE 80

struct Subcategory {
    long id;
    long category;
    char name[SUBCATEGORY_NAME_SIZE];
    CharInt code;
    CharInt operationCode;
};

class SubcategoryParser: public JsonObjectParser {
protected:
    int parseName(std::string *n) override;
    void parseValue(int field_id) override;
public:
    Subcategory subcategory;

    explicit SubcategoryParser(const char *file_name): JsonObjectParser(file_name) {}
};

class Subcategories: public JsonObjectArrayParser<Subcategory> {
    SubcategoryParser *parser;
public:
    explicit Subcategories(const char *data_folder, long capacity);

    inline ~Subcategories() {
        delete parser;
    }

    inline void parse() {
        parse_array(parser->parser);
    }

protected:
    Subcategory *create(JsonParser *p) override;
    long getId(Subcategory *value) override;
    bool isValid(Subcategory *value) override;
};

#endif
