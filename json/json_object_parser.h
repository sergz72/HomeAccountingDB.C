#ifndef H_JSON_OBJECT_PARSER_H
#define H_JSON_OBJECT_PARSER_H

#include "json_parser.h"

class JsonObjectParser {
    bool delete_parser;
protected:
    virtual int parseName(std::string &name) const = 0;
    virtual void parseValue(int field_id) = 0;
public:
    JsonParser *parser;

    explicit JsonObjectParser(const char *file_name) {
        parser = new JsonParser(file_name);
        delete_parser = true;
    }

    explicit JsonObjectParser(JsonParser *p) {
        parser = p;
        delete_parser = false;
    }

    virtual ~JsonObjectParser();

    void parse(bool call_expected);
};

#endif
