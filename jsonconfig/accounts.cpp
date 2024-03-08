#include "accounts.h"
#include <cstring>

#define ACCOUNT_ID 1
#define ACCOUNT_NAME 2
#define ACCOUNT_CURRENCY_NAME 3
#define ACCOUNT_ACTIVE_TO 4
#define ACCOUNT_IS_CASH 5

int AccountParser::parseName(std::string &n) const {
    if (n == "id")
        return ACCOUNT_ID;
    else if (n == "name")
        return ACCOUNT_NAME;
    else if (n == "valutaCode")
        return ACCOUNT_CURRENCY_NAME;
    else if (n == "activeTo")
        return ACCOUNT_ACTIVE_TO;
    else if (n == "isCash")
        return ACCOUNT_IS_CASH;

    return 0;
}

void AccountParser::parseValue(int fieldId) {
    long id;
    switch (fieldId) {
        case ACCOUNT_ID:
            id = parser->expectedInteger();
            if (id <= 0)
                throw std::runtime_error("invalid account id");
            account.id = id;
            break;
        case ACCOUNT_NAME:
            parser->expectedString();
            strncpy(account.name, parser->current.stringValue.c_str(), ACCOUNT_NAME_SIZE);
            break;
        case ACCOUNT_CURRENCY_NAME:
            parser->expectedString();
            if (parser->current.stringValue.length() != 3)
                throw std::runtime_error("incorrect currency");
            account.currency.intValue = 0;
            strcpy(account.currency.charValue, parser->current.stringValue.c_str());
            break;
        case ACCOUNT_ACTIVE_TO:
            account.activeTo = parse_date(parser);
            break;
        case ACCOUNT_IS_CASH:
            account.cashAccount = parser->expectedBool() ? 0 : 1;
        default:
            break;
    }
}

AccountsJsonSource::AccountsJsonSource(const char *dataFolder) : JsonObjectArrayParser<Account>() {
    auto fileName = std::string(dataFolder);
    fileName += "/accounts.json";
    parser = new AccountParser(fileName.c_str());
}

AccountsJsonSource::~AccountsJsonSource() {
    delete parser;
}

Account *AccountsJsonSource::create() {
    parser->parse(false);
    return &parser->account;
}

unsigned long AccountsJsonSource::getId(const Account *value) const {
    return value->id;
}

JsonParser* AccountsJsonSource::getParser() {
    return parser->parser;
}
