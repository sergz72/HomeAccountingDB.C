#ifndef H_SUBCATEGORIES_H
#define H_SUBCATEGORIES_H

#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"

#define SUBCATEGORY_NAME_SIZE 80

enum SubcategoryOperationCode {
    incm,
    expn,
    spcl
};

enum SubcategoryCode {
    comb,
    comc,
    fuel,
    prcn,
    incc,
    expc,
    exch,
    trfr,
    none
};

struct Subcategory {
    unsigned long id;
    unsigned long category;
    char name[SUBCATEGORY_NAME_SIZE];
    SubcategoryCode code;
    SubcategoryOperationCode operationCode;
};

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

    inline ~SubcategoriesJsonSource() {
        delete parser;
    }

protected:
    Subcategory *create() override;
    unsigned long getId(const Subcategory *value) const override;
    JsonParser *getParser() override;
};

class Subcategories: public ObjectArray<Subcategory> {
public:
    inline explicit Subcategories(ObjectArraySource<Subcategory> *source, unsigned long capacity):
            ObjectArray<Subcategory>(source, capacity) {}
protected:
    bool isValid(const Subcategory *value) const override;
};

#endif
