#include "common.h"

long parse_date(JsonParser *parser) {
    parser->nextToken();
    if (parser->current.isNull()) {
        return 0;
    }
    parser->is('[');
    long value = parser->expectedInteger() * 10000;
    parser->expected(',');
    value += parser->expectedInteger() * 100;
    parser->expected(',');
    value += parser->expectedInteger();
    parser->expected(']');
    return value;
}