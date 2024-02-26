#ifndef H_COMMON_H
#define H_COMMON_H

#include "../json/json_parser.h"

union CharInt {
    char char_value[sizeof(long)];
    long int_value;
};

unsigned long parse_date(JsonParser *parser);

#endif
