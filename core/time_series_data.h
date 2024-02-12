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
    T **data;

    virtual T* createObject() = 0;
    virtual long getDataIndex(long date) = 0;
public:
    inline explicit TimeSeriesData(std::string _path, long _capacity) {
        path = std::move(_path);
        capacity = _capacity;
        data = new T*[_capacity]();
    }

    inline ~TimeSeriesData() {
        delete data;
    }

    inline void load_json() {
        for (const auto & entry1 : std::filesystem::directory_iterator(path)) {
            if (entry1.is_directory()) {
                auto date = atol(entry1.path().filename().c_str());
                auto data_index = getDataIndex(date);
                if (data_index < 0 || data_index >= capacity)
                    throw std::runtime_error("data index is out of bounds");
                auto obj = data[data_index];
                if (obj == nullptr) {
                    obj = createObject();
                    data[data_index] = obj;
                }
                for (const auto & entry2 : std::filesystem::directory_iterator(entry1)) {
                    obj->parse(entry2.path().c_str());
                }
            }
        }
    }
};

#endif
