#ifndef H_TIME_SERIES_DATA_H
#define H_TIME_SERIES_DATA_H

#include "../json/json_object_array_parser.h"
#include <concepts>
#include <filesystem>
#include <utility>

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

template<typename K, Derived<JsonObjectArrayParser<K>> T>
class TimeSeriesData {
    std::string path;
protected:
    long capacity;
    long count;
    T **data;

    virtual T* createObject() = 0;
    virtual long getDataIndex(long date) = 0;
public:
    inline explicit TimeSeriesData(std::string _path, long _capacity) {
        path = std::move(_path);
        capacity = _capacity;
        count = 0;
        data = new T*[_capacity]();
    }

    inline ~TimeSeriesData() {
        delete data;
    }

    inline long validateDataIndex(long date) {
        auto data_index = getDataIndex(date);
        if (data_index < 0 || data_index >= capacity)
            throw std::runtime_error("data index is out of bounds");
        return data_index;
    }

    inline void load_json() {
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                auto date = atol(entry1.path().filename().c_str());
                auto data_index = validateDataIndex(date);
                if (data_index >= count)
                    count = data_index + 1;
                auto obj = data[data_index];
                if (obj == nullptr) {
                    obj = createObject();
                    data[data_index] = obj;
                }
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    obj->parse(entry2.path().c_str(), date);
                }
            }
        }
    }
};

#endif
