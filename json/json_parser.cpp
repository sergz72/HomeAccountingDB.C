#include <iostream>
#include <fstream>
#include <sstream>
#include "json_parser.h"
#include <cstring>

void JsonToken::print() const {
    switch (typ) {
        case eof:
            std::cout << "EOF" << std::endl;
            break;
        case character:
            std::cout << "CHAR " << charValue << std::endl;
            break;
        case str:
            std::cout <<"STR " << stringValue << std::endl;
            break;
        case name:
            std::cout <<"NAME " << stringValue << std::endl;
            break;
        case integer:
            std::cout <<"INT " << intValue << std::endl;
            break;
        case real:
            std::cout <<"REAL " << doubleValue << std::endl;
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
                        current.stringValue.clear();
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
                            current.stringValue = c;
                        }
                        break;
                }
                break;
            case name:
                if (c >= 'a' && c <= 'z') {
                    current.stringValue += c;
                } else {
                    position--;
                    return;
                }
                break;
            case str:
                if (c == '"') {
                    return;
                } else {
                    current.stringValue += c;
                }
                break;
            case integer:
                if (c >= '0' && c <= '9')
                    current.intValue = current.intValue * 10 + c - '0';
                else if (c == '.') {
                    current.typ = real;
                    divider = 1;
                } else {
                    position--;
                    current.intValue *= sign;
                    return;
                }
                break;
            case real:
                if (c >= '0' && c <= '9') {
                    current.intValue = current.intValue * 10 + c - '0';
                    divider *= 10;
                } else {
                    position--;
                    current.doubleValue = (double)current.intValue * sign / divider;
                    return;
                }
                break;
            case eof:
                return;
        }
    }
    current.typ = eof;
}
