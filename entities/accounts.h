#ifndef H_ACCOUNTS_H
#define H_ACCOUNTS_H

#include <map>
#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "common.h"

#define ACCOUNT_NAME_SIZE 80

struct Account {
    long id;
    char name[ACCOUNT_NAME_SIZE];
    CharInt currency;
    long active_to;
    long cashAccount;
};

class AccountParser: public JsonObjectParser {
protected:
    int parseName(std::string *n) override;
    void parseValue(int field_id) override;
public:
    Account account;

    explicit AccountParser(const char * file_name): JsonObjectParser(file_name) {}
};

class Accounts: public JsonObjectArrayParser<Account> {
    AccountParser *parser;
    std::map<long, long> cash_accounts;
public:
    explicit Accounts(const char *data_folder, long capacity);

    void buildCashAccounts();

    inline ~Accounts() {
        delete parser;
    }

    inline void parse() {
        parse_array(parser->parser);
        buildCashAccounts();
    }

protected:
    Account *create(JsonParser *p) override;
    long getId(Account *value) override;
    bool isValid(Account *value) override;
};

#endif
