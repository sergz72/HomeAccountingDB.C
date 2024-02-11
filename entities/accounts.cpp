#include "accounts.h"

#define ACCOUNT_ID 1
#define ACCOUNT_NAME 2
#define ACCOUNT_CURRENCY_NAME 3
#define ACCOUNT_ACTIVE_TO 4
#define ACCOUNT_IS_CASH 5

int AccountParser::parseName(std::string *n) {
    if (*n == "id")
        return ACCOUNT_ID;
    else if (*n == "name")
        return ACCOUNT_NAME;
    else if (*n == "valutaCode")
        return ACCOUNT_CURRENCY_NAME;
    else if (*n == "activeTo")
        return ACCOUNT_ACTIVE_TO;
    else if (*n == "isCash")
        return ACCOUNT_IS_CASH;

    return 0;
}

void AccountParser::parseValue(int field_id) {
    switch (field_id) {
        case ACCOUNT_ID:
            account.id = parser->expectedInteger();
            break;
        case ACCOUNT_NAME:
            parser->expectedString();
            strncpy(account.name, parser->current.string_value.c_str(), ACCOUNT_NAME_SIZE);
            break;
        case ACCOUNT_CURRENCY_NAME:
            parser->expectedString();
            if (parser->current.string_value.length() != 3)
                throw std::runtime_error("incorrect currency");
            account.currency.int_value = 0;
            strcpy(account.currency.char_value, parser->current.string_value.c_str());
            break;
        case ACCOUNT_ACTIVE_TO:
            account.active_to = parse_date(parser);
            break;
        case ACCOUNT_IS_CASH:
            account.cashAccount = parser->expectedBool() ? -1 : 0;
        default:
            break;
    }
}

Accounts::Accounts(const char *data_folder, long capacity) : JsonObjectArrayParser<Account>(capacity) {
    auto file_name = std::string(data_folder);
    file_name += "/accounts.json";
    parser = new AccountParser(file_name.c_str());
}

Account *Accounts::create(JsonParser *p) {
    parser->parse(false);
    return &parser->account;
}

long Accounts::getId(Account *value) {
    return value->id;
}

bool Accounts::isValid(Account *value) {
    return value->id > 0;
}

void Accounts::buildCashAccounts() {
    auto el = array;
    for (long i = 0; i < count; i++) {
        if (isValid(el)) {
            if (el->cashAccount == -1) {
                if (cash_accounts.contains(el->currency.int_value))
                    throw std::runtime_error("duplicate cash account");
                cash_accounts.insert({el->currency.int_value, el->id});
            }
        }
        el++;
    }

    el = array;
    for (long i = 0; i < count; i++) {
        if (isValid(el)) {
            if (el->cashAccount == 0) {
                if (!cash_accounts.contains(el->currency.int_value))
                    throw std::runtime_error("cash account not found");
                el->cashAccount = cash_accounts[el->currency.int_value];
            }
        }
        el++;
    }
}
