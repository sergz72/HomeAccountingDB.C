#ifndef H_TIME_SERIES_DATA_H
#define H_TIME_SERIES_DATA_H

#include "../json/json_object_array_parser.h"
#include <concepts>
#include <filesystem>
#include <utility>
#include <map>

template<typename T>
class DatedSource {
public:
    virtual T* load(std::vector<std::string> &files) = 0;
    virtual long getDate(const char *folder, const std::filesystem::directory_entry &entry) = 0;
};

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

template<typename K, Derived<JsonObjectArrayParser<K>> T>
class TimeSeriesData {
    std::string path;

    inline void addToMap(
            DatedSource<T> &source,
            std::map<unsigned long, std::vector<std::string>> &map,
            const char *folder,
            const std::filesystem::directory_entry &entry)
    {
        auto date = source->getDate(folder, entry);
        auto key = validateDataIndex(date);
        map[key].add(entry.path().string());
    }
protected:
    unsigned long capacity;
    unsigned long count;
    T **data;

    virtual T* createObject() const = 0;
    virtual long calculateKey(long date) const = 0;
public:
    inline explicit TimeSeriesData(DatedSource<T> &source, unsigned long _capacity) {
        path = std::move(_path);
        capacity = _capacity;
        count = 0;
        data = new T*[_capacity]();
    }

    inline ~TimeSeriesData() {
        delete data;
    }

    inline unsigned long validateDataIndex(long date) {
        auto key = calculateKey(date);
        if (key < 0 || key >= capacity)
            throw std::runtime_error("key is out of bounds");
        return key;
    }

    inline void load(DatedSource<T> &source, const char &folder) {
        std::map<unsigned long, std::vector<std::string>> fileMap;
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    addToMap(source, fileMap, entry1.path().filename().c_str(), entry2);
                }
            }
            else
                addToMap(fileMap, nullptr, entry1);
        }
        for (auto const& entry: fileMap) {

        }
        /*if (key >= count)
            count = key + 1;
        auto obj = data[key];
        if (obj == nullptr) {
            obj = createObject();
            data[key] = obj;
        }*/
    }
};

#endif
