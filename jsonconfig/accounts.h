#ifndef H_ACCOUNTS_CONFIG_H
#define H_ACCOUNTS_CONFIG_H

#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "../entities/accounts.h"

class AccountParser: public JsonObjectParser {
protected:
    int parseName(std::string &n) const override;
    void parseValue(int fieldId) override;
public:
    Account account;

    explicit AccountParser(const char * fileName): JsonObjectParser(fileName) {}
};

class AccountsJsonSource: public JsonObjectArrayParser<Account> {
    AccountParser *parser;
public:
    explicit AccountsJsonSource(const char *dataFolder);
    ~AccountsJsonSource() override;
protected:
    Account *create() override;
    unsigned long getId(const Account *value) const override;
    JsonParser *getParser() override;
};

#endif
