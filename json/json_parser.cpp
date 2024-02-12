#include <iostream>
#include <fstream>
#include <sstream>
#include "json_parser.h"

void JsonToken::print() const {
    switch (typ) {
        case eof:
            std::cout << "EOF" << std::endl;
            break;
        case character:
            std::cout << "CHAR " << char_value << std::endl;
            break;
        case str:
            std::cout <<"STR " << string_value << std::endl;
            break;
        case name:
            std::cout <<"NAME " << string_value << std::endl;
            break;
        case integer:
            std::cout <<"INT " << int_value << std::endl;
            break;
        case real:
            std::cout <<"REAL " << double_value << std::endl;
            break;
    }
}

JsonParser::JsonParser(const char *file_name) {
    std::ifstream file;
    file.open(file_name);
    if (!file.is_open())
        throw std::runtime_error(strerror(errno));
    std::stringstream buffer;
    if (!(buffer << file.rdbuf()))
        throw std::runtime_error(strerror(errno));
    contents = buffer.str();
    position = 0;
    length = contents.length();
    divider = 1;
}

void JsonParser::nextToken() {
    current.typ = character;
    long sign = 1;
    while (position < length) {
        auto c = contents[position++];
        switch (current.typ) {
            case character:
                switch (c) {
                    case '[':
                    case '{':
                    case ']':
                    case '}':
                    case ':':
                    case ',':
                        current.setChar(c);
                        return;
                    case '"':
                        current.typ = str;
                        current.string_value.clear();
                        break;
                    default:
                        if (c == '-') {
                            current.setInteger(0);
                            sign = -1;
                        }
                        else if (c >= '0' && c <= '9') {
                            current.setInteger(c - '0');
                        } else if (c > 0x20) {
                            current.typ = name;
                            current.string_value = c;
                        }
                        break;
                }
                break;
            case name:
                if (c >= 'a' && c <= 'z') {
                    current.string_value += c;
                } else {
                    position--;
                    return;
                }
                break;
            case str:
                if (c == '"') {
                    return;
                } else {
                    current.string_value += c;
                }
                break;
            case integer:
                if (c >= '0' && c <= '9')
                    current.int_value = current.int_value * 10 + c - '0';
                else if (c == '.') {
                    current.typ = real;
                    divider = 1;
                } else {
                    position--;
                    current.int_value *= sign;
                    return;
                }
                break;
            case real:
                if (c >= '0' && c <= '9') {
                    current.int_value = current.int_value * 10 + c - '0';
                    divider *= 10;
                } else {
                    position--;
                    current.double_value = (double)current.int_value * sign / divider;
                    return;
                }
                break;
            case eof:
                return;
        }
    }
    current.typ = eof;
}
