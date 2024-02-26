#ifndef H_ACCOUNTS_H
#define H_ACCOUNTS_H

#include <map>
#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "../core/object_array.h"
#include "common.h"

#define ACCOUNT_NAME_SIZE 80

struct Account {
    unsigned long id;
    char name[ACCOUNT_NAME_SIZE];
    CharInt currency;
    unsigned long active_to;
    unsigned long cashAccount;
};

class AccountParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) const override;
    void parseValue(int field_id) override;
public:
    Account account;

    explicit AccountParser(const char * file_name): JsonObjectParser(file_name) {}
};

class AccountsJsonSource: public JsonObjectArrayParser<Account> {
    AccountParser *parser;
public:
    explicit AccountsJsonSource(const char *data_folder);
    ~AccountsJsonSource() override;
protected:
    Account *create() override;
    unsigned long getId(const Account *value) const override;
    JsonParser *getParser() override;
};

class Accounts: public ObjectArray<Account> {
public:
    inline explicit Accounts(ObjectArraySource<Account> *source, unsigned long capacity):
        ObjectArray<Account>(source, capacity) {
        buildCashAccounts();
    }

    void buildCashAccounts();

    unsigned long getCashAccount(unsigned long id);
protected:
    bool isValid(const Account *value) const override;
};

#endif
