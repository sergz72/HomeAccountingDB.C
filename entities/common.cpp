#include "common.h"

unsigned long parse_date(JsonParser *parser) {
    parser->nextToken();
    if (parser->current.isNull()) {
        return 0;
    }
    parser->is('[');
    long value = parser->expectedInteger() * 10000;
    if (value <= 0)
        throw std::runtime_error("wrong year value");
    parser->expected(',');
    long month = parser->expectedInteger();
    if (month <= 0 || month > 12)
        throw std::runtime_error("wrong month value");
    value += month * 100;
    parser->expected(',');
    long day = parser->expectedInteger();
    if (day <= 0 || day > 31)
        throw std::runtime_error("wrong day value");
    value += day;
    parser->expected(']');
    return value;
}