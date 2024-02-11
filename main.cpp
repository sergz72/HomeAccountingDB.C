#include <iostream>
#include <chrono>
#include <filesystem>
#include "json/json_parser.h"
#include "entities/accounts.h"
#include "entities/subcategories.h"
#include "entities/finance_operations.h"

void usage() {
    std::cout << "Usage: home_accounting_db file_name" << std::endl;
}

/*void printAll(JsonParser *parser) {
    parser->nextToken();
    while (parser->current.typ != eof) {
        parser->current.print();
        parser->nextToken();
    }
}*/

int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
        return 1;
    }

    try {
        auto begin = std::chrono::steady_clock::now();
        auto accounts = new Accounts(argv[1], 300);
        accounts->parse();
        auto subcategories = new Subcategories(argv[1], 2000);
        subcategories->parse();
        auto path = std::string(argv[1]).append("/dates");
        auto operations = new FinanceOperations(20000);
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                auto date = atol(entry1.path().filename().c_str());
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    operations->parse(entry2.path().c_str());
                }
            }
        }
        auto end = std::chrono::steady_clock::now();
        delete accounts;
        delete subcategories;
        std::cout << "Elapsed " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " us" << std::endl;
    } catch (const std::runtime_error &error) {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
