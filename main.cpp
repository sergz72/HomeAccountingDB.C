#include <iostream>
#include <chrono>
#include "db.h"

void usage() {
    std::cout << "Usage: home_accounting_db data_folder_path date" << std::endl;
}

/*void printAll(JsonParser *parser) {
    parser->nextToken();
    while (parser->current.typ != eof) {
        parser->current.print();
        parser->nextToken();
    }
}*/

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
        return 1;
    }

    try {
        auto date = atol(argv[2]);
        auto begin = std::chrono::steady_clock::now();
        auto db = new DB(argv[1], 2012, 6, 300, 2000, 300, 1000, nullptr);
        db->load();
        auto end = std::chrono::steady_clock::now();
        std::cout << "Database loaded in " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " us" << std::endl;
        begin = std::chrono::steady_clock::now();
        db->calculateTotals(0);
        end = std::chrono::steady_clock::now();
        std::cout << "Totals calculation finished in " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " us" << std::endl;
        db->printChanges(date);
    } catch (const std::runtime_error &error) {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
