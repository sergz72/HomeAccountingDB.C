#include "json_db_config.h"
#include "accounts.h"
#include "subcategories.h"
#include "finance_operations.h"

class JsonFinanceOperationsSource: public JsonObjectArrayParser<FinanceOperation> {
    FinanceOperationParser *parser;
    std::vector<FileWithDate> files;
    long propertiesCapacity;
public:
    explicit JsonFinanceOperationsSource(const std::vector<FileWithDate>& _files, long _propertiesCapacity) {
        files = _files;
        propertiesCapacity = _propertiesCapacity;
    }

    unsigned long load(FinanceOperation *array, unsigned long &count, unsigned long capacity) override;

    FinanceOperation *create() override;
    unsigned long getId(const FinanceOperation *value) const override;
    JsonParser *getParser() override;
};

FinanceOperation *JsonFinanceOperationsSource::create() {
    parser->parse(false);
    return &parser->operation;
}

unsigned long JsonFinanceOperationsSource::getId(const FinanceOperation *value) const {
    return 0;
}

JsonParser *JsonFinanceOperationsSource::getParser() {
    return parser->parser;
}

unsigned long JsonFinanceOperationsSource::load(FinanceOperation *array, unsigned long &count, unsigned long capacity) {
    unsigned long added = 0;
    for (const auto& file: files) {
        parser = new FinanceOperationParser(file.fileName.c_str(), propertiesCapacity);
        auto from = count;
        added += JsonObjectArrayParser::load(array, count, capacity);
        delete parser;
        while (from < count)
            array[from++].date = file.date;
    }
    return added;
}

class JsonDatedSource: public DatedSource<FinanceOperations> {
    long capacity;
    long propertiesCapacity;
public:
    inline explicit JsonDatedSource(long _capacity, long _propertiesCapacity) {
        capacity = _capacity;
        propertiesCapacity = _propertiesCapacity;
    }

    FinanceOperations* load(const std::vector<FileWithDate> &files) override;
    long getDate(const char *folder, const std::filesystem::directory_entry &entry) override;
    std::vector<FileWithDate> &getFiles(unsigned long date) override;
};

FinanceOperations* JsonDatedSource::load(const std::vector<FileWithDate> &files) {
    auto operationsSource = new JsonFinanceOperationsSource(files, propertiesCapacity);
    return new FinanceOperations(operationsSource, capacity);
}

long JsonDatedSource::getDate(const char *folder, const std::filesystem::directory_entry &entry) {
    return atol(folder);
}

std::vector<FileWithDate> &JsonDatedSource::getFiles(unsigned long date) {
    throw std::runtime_error("JsonDatedSource::getFiles is not implemented");
}

ObjectArraySource<Account> *JsonDBConfig::getAccountsSource() {
    return new AccountsJsonSource(baseFolder);
}

ObjectArraySource<Subcategory> *JsonDBConfig::getSubcategoriesSource() {
    return new SubcategoriesJsonSource(baseFolder);
}

DatedSource<FinanceOperations> *JsonDBConfig::getMainDataSource() {
    return new JsonDatedSource(capacity, propertiesCapacity);
}
