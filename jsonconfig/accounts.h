#ifndef H_ACCOUNTS_CONFIG_H
#define H_ACCOUNTS_CONFIG_H

#include "../json/json_object_array_parser.h"
#include "../json/json_object_parser.h"
#include "../entities/accounts.h"

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

#endif
