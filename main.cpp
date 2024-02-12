#include <iostream>
#include <chrono>
#include "db.h"

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
        auto db = new DB(argv[1], 2012, 6, 300, 2000, 200, 1000);
        db->load_json();
        auto end = std::chrono::steady_clock::now();
        std::cout << "Elapsed " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << " us" << std::endl;
    } catch (const std::runtime_error &error) {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
