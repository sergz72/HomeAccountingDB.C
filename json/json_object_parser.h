#ifndef H_JSON_OBJECT_PARSER_H
#define H_JSON_OBJECT_PARSER_H

#include "json_parser.h"

class JsonObjectParser {
    bool deleteParser;
protected:
    virtual int parseName(std::string &name) const = 0;
    virtual void parseValue(int fieldId) = 0;
public:
    JsonParser *parser;

    explicit JsonObjectParser(const char *fileName) {
        parser = new JsonParser(fileName);
        deleteParser = true;
    }

    explicit JsonObjectParser(JsonParser *p) {
        parser = p;
        deleteParser = false;
    }

    virtual ~JsonObjectParser();

    void parse(bool call_expected);
};

#endif
