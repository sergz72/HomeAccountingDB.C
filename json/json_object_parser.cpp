#include "json_object_parser.h"

void JsonObjectParser::parse(bool call_expected) {
    if (call_expected)
        parser->expected('{');
    else
        parser->is('{');
    while (parser->current.typ != eof) {
        parser->expectedString();
        auto field_id = parseName(parser->current.string_value);
        if (field_id == 0) {
            throw std::runtime_error("unknown field name " + parser->current.string_value);
        }
        parser->expected(':');
        parseValue(field_id);
        char c = parser->expectedChar();
        if (c == '}')
            break;
        if (c != ',') {
            throw std::runtime_error(", expected");
        }
    }
}
