#ifndef H_JSON_PARSER_H
#define H_JSON_PARSER_H

#include <stdexcept>
#include <string>

enum JsonTokenType {
    eof,
    str,
    integer,
    real,
    character,
    name
};

struct JsonToken {
    JsonTokenType typ;
    char charValue;
    std::string stringValue;
    long intValue;
    double doubleValue;
public:
    JsonToken() {
        typ = character;
        charValue = 0;
        intValue = 0;
        doubleValue = 0;
    }

    void setChar(char c) {
        typ = character;
        charValue = c;
    }

    inline void setInteger(long l) {
        typ = integer;
        intValue = l;
    }

    inline bool isNull() const {
        return typ == name && stringValue == "null";
    }

    void print() const;
};

class JsonParser {
    std::string contents;
    unsigned long position;
    unsigned long length;
    long divider;
public:
    JsonToken current;

    explicit JsonParser(const char *fileName);
    void nextToken();

    void expected(char c) {
        nextToken();
        is(c);
    }

    void is(char c) const {
        if (current.typ != character || current.charValue != c)
            throw std::runtime_error(std::string(" expected").insert(0, 1, c));
    }

    char expectedChar() {
        nextToken();
        if (current.typ != character)
            throw std::runtime_error("char expected");
        return current.charValue;
    }

    void expectedString() {
        nextToken();
        isString();
    }

    void isString() const {
        if (current.typ != str)
            throw std::runtime_error("string expected");
    }

    long expectedInteger() {
        nextToken();
        return isInteger();
    }

    long isInteger() const {
        if (current.typ != integer)
            throw std::runtime_error("integer expected");
        return current.intValue;
    }

    inline void expectedName() {
        nextToken();
        if (current.typ != name)
            throw std::runtime_error("name expected");
    }

    inline bool expectedBool() {
        expectedName();
        if (current.stringValue == "true")
            return true;
        else if (current.stringValue == "false")
            return false;
        else
            throw std::runtime_error("bool expected");
    }
};

#endif
