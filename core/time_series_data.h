#ifndef H_TIME_SERIES_DATA_H
#define H_TIME_SERIES_DATA_H

#include <filesystem>
#include <map>
#include <utility>
#include <vector>
#include "lru.h"

struct FileWithDate {
    std::string fileName;
    unsigned long date;
};

template<typename T>
class DatedSource {
public:
    virtual T* load(const std::vector<FileWithDate> &files) = 0;
    virtual long getDate(const char *folder, const std::filesystem::directory_entry &entry) = 0;
};

template<typename T>
class TimeSeriesData: Lru<T> {
    std::string path;
    DatedSource<T> *source;

    unsigned long capacity;
    unsigned long maxActiveItems;
    unsigned long activeItems;
    LruItem<T> **data;

    void addToMap(
            std::map<unsigned long, std::vector<FileWithDate>> &map,
            const char *folder,
            const std::filesystem::directory_entry &entry)
    {
        auto date = source->getDate(folder, entry);
        auto key = validateDataIndex(date);
        map[key].push_back(FileWithDate{entry.path().string(), (unsigned long)date});
    }
protected:
    unsigned long count;

    T* get(unsigned long idx) {
        auto d = data[idx];
        return d == nullptr ? nullptr : d->data;
    }

    virtual long calculateKey(long date) const = 0;
public:
    explicit TimeSeriesData(std::string &folder, DatedSource<T> *_source, unsigned long _capacity,
                            unsigned long _maxActiveItems) {
        path = std::move(folder);
        source = _source;
        capacity = _capacity;
        count = activeItems = 0;
        maxActiveItems = _maxActiveItems;
        data = new LruItem<T>*[_capacity]();
    }

    virtual ~TimeSeriesData() {
        delete data;
    }

    unsigned long validateDataIndex(long date) const {
        auto key = calculateKey(date);
        if (key < 0 || key >= capacity)
            throw std::runtime_error("key is out of bounds");
        return key;
    }

    void loadAll() {
        std::map<unsigned long, std::vector<FileWithDate>> fileMap;
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    addToMap(fileMap, entry1.path().filename().c_str(), entry2);
                }
            }
            else
                addToMap(fileMap, nullptr, entry1);
        }
        for (auto const& entry: fileMap) {
            auto object = source->load(entry.second);
            if (entry.first >= count)
                count = entry.first + 1;
            data[entry.first] = this->lruAdd(object);
        }
    }

    void init() {
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    auto date = source->getDate(entry1.path().filename().c_str(), entry2);
                    auto key = validateDataIndex(date);
                    if (data[key] == nullptr)
                        data[key] = new LruItem<T>();
                }
            }
            else {
                auto date = source->getDate(nullptr, entry1);
                auto key = validateDataIndex(date);
                if (data[key] == nullptr)
                    data[key] = new LruItem<T>();
            }
        }
    }
};

#endif
