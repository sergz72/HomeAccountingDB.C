#ifndef H_JSON_OBJECT_ARRAY_PARSER_H
#define H_JSON_OBJECT_ARRAY_PARSER_H

#include <cstdlib>
#include "json_parser.h"
#include "../core/object_array.h"

template<typename T>
class JsonObjectArrayParser: public ObjectArraySource<T> {
protected:
    virtual T *create() = 0;
    virtual unsigned long getId(const T *value) const = 0;
    virtual JsonParser *getParser() = 0;

    unsigned long parseArray(T *array, unsigned long &count, unsigned long capacity,
                              bool callExpected = true) {
        auto p = getParser();
        unsigned long added = 0;
        if (callExpected)
            p->expected('[');
        else
            p->is('[');
        p->nextToken();
        while (p->current.typ != character || p->current.charValue != ']') {
            T *value = create();
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
            if (p->current.charValue == ',')
                p->nextToken();
            else if (p->current.charValue != ']')
                throw std::runtime_error(", or ] expected");
            added++;
        }
        return added;
    }

    virtual unsigned long load(T *array, unsigned long &count, unsigned long capacity) {
        return parseArray(array, count, capacity);
    }

    virtual void save(T *array, unsigned long count, void *to) {
    }
};

#endif
