#ifndef H_SUBCATEGORIES_CONFIG_H
#define H_SUBCATEGORIES_CONFIG_H

#include "../entities/subcategories.h"
#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"

class SubcategoryParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) const override;
    void parseValue(int field_id) override;
public:
    Subcategory subcategory;

    explicit SubcategoryParser(const char *file_name): JsonObjectParser(file_name) {}
};

class SubcategoriesJsonSource: public JsonObjectArrayParser<Subcategory> {
    SubcategoryParser *parser;
public:
    explicit SubcategoriesJsonSource(const char *data_folder);

    ~SubcategoriesJsonSource() override;
protected:
    Subcategory *create() override;
    unsigned long getId(const Subcategory *value) const override;
    JsonParser *getParser() override;
};


#endif
