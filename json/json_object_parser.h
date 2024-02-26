#ifndef H_JSON_OBJECT_PARSER_H
#define H_JSON_OBJECT_PARSER_H

#include "json_parser.h"

class JsonObjectParser {
protected:
    virtual int parseName(std::string &name) const = 0;
    virtual void parseValue(int field_id) = 0;
public:
    JsonParser *parser;

    inline explicit JsonObjectParser(const char *file_name) {
        parser = new JsonParser(file_name);
    }

    inline explicit JsonObjectParser(JsonParser *p) {
        parser = p;
    }

    inline ~JsonObjectParser() {
        delete parser;
    }

    void parse(bool call_expected);
};

#endif
