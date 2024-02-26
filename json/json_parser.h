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
    char char_value;
    std::string string_value;
    long int_value;
    double double_value;
public:
    inline JsonToken() {
        typ = character;
        char_value = 0;
        int_value = 0;
        double_value = 0;
    }

    inline void setChar(char c) {
        typ = character;
        char_value = c;
    }

    inline void setInteger(long l) {
        typ = integer;
        int_value = l;
    }

    inline bool isNull() const {
        return typ == name && string_value == "null";
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

    explicit JsonParser(const char *file_name);
    void nextToken();

    inline void expected(char c) {
        nextToken();
        is(c);
    }

    inline void is(char c) const {
        if (current.typ != character || current.char_value != c)
            throw std::runtime_error(std::string(" expected").insert(0, 1, c));
    }

    inline char expectedChar() {
        nextToken();
        if (current.typ != character)
            throw std::runtime_error("char expected");
        return current.char_value;
    }

    inline void expectedString() {
        nextToken();
        isString();
    }

    inline void isString() const {
        if (current.typ != str)
            throw std::runtime_error("string expected");
    }

    inline long expectedInteger() {
        nextToken();
        return isInteger();
    }

    inline long isInteger() const {
        if (current.typ != integer)
            throw std::runtime_error("integer expected");
        return current.int_value;
    }

    inline void expectedName() {
        nextToken();
        if (current.typ != name)
            throw std::runtime_error("name expected");
    }

    inline bool expectedBool() {
        expectedName();
        if (current.string_value == "true")
            return true;
        else if (current.string_value == "false")
            return false;
        else
            throw std::runtime_error("bool expected");
    }
};

#endif
