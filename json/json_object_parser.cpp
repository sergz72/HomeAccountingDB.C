#include "json_object_parser.h"

JsonObjectParser::~JsonObjectParser() {
    if (deleteParser)
        delete parser;
}

void JsonObjectParser::parse(bool call_expected) {
    if (call_expected)
        parser->expected('{');
    else
        parser->is('{');
    while (parser->current.typ != eof) {
        parser->expectedString();
        auto fieldId = parseName(parser->current.stringValue);
        if (fieldId == 0) {
            throw std::runtime_error("unknown field name " + parser->current.stringValue);
        }
        parser->expected(':');
        parseValue(fieldId);
        char c = parser->expectedChar();
        if (c == '}')
            break;
        if (c != ',') {
            throw std::runtime_error(", expected");
        }
    }
}
