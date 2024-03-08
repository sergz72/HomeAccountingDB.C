#ifndef H_COMMON_H
#define H_COMMON_H

#include "../json/json_parser.h"

union CharInt {
    char charValue[sizeof(long)];
    long intValue;
};

unsigned long parse_date(JsonParser *parser);

#endif
