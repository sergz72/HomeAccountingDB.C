#ifndef H_JSON_OBJECT_ARRAY_PARSER_H
#define H_JSON_OBJECT_ARRAY_PARSER_H

#include <cstdlib>
#include "json_parser.h"

template<typename T>
class JsonObjectArrayParser {
protected:
    T *array;
    long capacity;
    long count;

    virtual T *create(JsonParser *p) = 0;
    virtual long getId(T *value) = 0;

    void parse_array(JsonParser *p, bool call_expected = true) {
        if (call_expected)
            p->expected('[');
        else
            p->is('[');
        p->nextToken();
        while (p->current.typ != character || p->current.char_value != ']') {
            T *value = create(p);
            long id = getId(value);
            if (id > 0) {
                if (id >= capacity)
                    throw std::runtime_error("parse_array: index is out of bounds");
                array[id] = *value;
                count = id + 1;
            }
            else
                array[count++] = *value;
            p->expectedChar();
            if (p->current.char_value == ',')
                p->nextToken();
            else if (p->current.char_value != ']')
                throw std::runtime_error(", or ] expected");
        }
    }
public:
    inline explicit JsonObjectArrayParser(long _capacity) {
        array = (T*)calloc(_capacity, sizeof(T));
        count = 0;
        capacity = _capacity;
    }

    inline ~JsonObjectArrayParser() {
        free(array);
    }

    T *get(long id) {
        if (id < 0 || id >= capacity)
            throw std::runtime_error("get: index is out of bounds");
        T *element = &array[id];
        if (!isValid(element))
            throw std::runtime_error("invalid element id");
        return element;
    }

    inline long getCount() {
        return count;
    }

    virtual bool isValid(T *value) = 0;
};

#endif
