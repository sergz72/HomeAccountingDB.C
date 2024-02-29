#ifndef H_FINANCE_OPERATIONS_CONFIG_H
#define H_FINANCE_OPERATIONS_CONFIG_H

#include "../json/json_parser.h"
#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "../entities/finance_operations.h"

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

    unsigned long load(FinOpProperty *array, unsigned long &count, unsigned long capacity) override {
        return parse_array(array, count, capacity, false);
    }

    ~FinOpPropertiesJsonSource() override;

protected:
    FinOpProperty *create() override;
    unsigned long getId(const FinOpProperty *value) const override;
    JsonParser *getParser() override;
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

#endif
