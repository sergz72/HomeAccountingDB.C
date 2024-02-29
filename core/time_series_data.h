#ifndef H_TIME_SERIES_DATA_H
#define H_TIME_SERIES_DATA_H

#include "object_array.h"
#include <filesystem>
#include <map>
#include <utility>
#include <vector>

struct FileWithDate {
    std::string file_name;
    unsigned long date;
};

template<typename T>
class DatedSource {
public:
    virtual T* load(const std::vector<FileWithDate> &files) = 0;
    virtual long getDate(const char *folder, const std::filesystem::directory_entry &entry) = 0;
};

template<typename T>
class TimeSeriesData {
    std::string path;
    DatedSource<T> *source;

    inline void addToMap(
            std::map<unsigned long, std::vector<std::string>> &map,
            const char *folder,
            const std::filesystem::directory_entry &entry)
    {
        auto date = source->getDate(folder, entry);
        auto key = validateDataIndex(date);
        map[key].push_back(entry.path().string());
    }
protected:
    unsigned long capacity;
    unsigned long count;
    T **data;

    virtual long calculateKey(long date) const = 0;
public:
    inline explicit TimeSeriesData(std::string &folder, DatedSource<T> *_source, unsigned long _capacity) {
        path = std::move(folder);
        source = _source;
        capacity = _capacity;
        count = 0;
        data = new T*[_capacity]();
    }

    virtual ~TimeSeriesData() {
        delete data;
    }

    unsigned long validateDataIndex(long date) {
        auto key = calculateKey(date);
        if (key < 0 || key >= capacity)
            throw std::runtime_error("key is out of bounds");
        return key;
    }

    void load() {
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
            data[entry.first] = object;
        }
    }
};

#endif
