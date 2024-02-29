#ifndef H_OBJECT_ARRAY_H
#define H_OBJECT_ARRAY_H

#include <cstdlib>
#include <stdexcept>

template<typename T>
class ObjectArraySource {
public:
    virtual unsigned long load(T *array, unsigned long &count, unsigned long capacity) = 0;
    virtual void save(T *array, unsigned long count, void *to) = 0;
    virtual ~ObjectArraySource() = default;
};

template<typename T>
class ObjectArray {
protected:
    T *array;
    unsigned long capacity;
    unsigned long count;
    unsigned long active_count;

    inline explicit ObjectArray(ObjectArraySource<T> *source, unsigned long _capacity, bool deleteSource = true) {
        array = (T*)calloc(_capacity, sizeof(T));
        capacity = _capacity;
        count = 0;
        active_count = source->load(array, count, _capacity);
        if (deleteSource)
            delete source;
    }

    virtual ~ObjectArray() {
        free(array);
    }

    virtual bool isValid(const T *value) const = 0;
public:
    T *get(unsigned long id) const {
        if (id >= capacity)
            throw std::runtime_error("get: index is out of bounds");
        T *element = &array[id];
        if (!isValid(element))
            throw std::runtime_error("invalid element id");
        return element;
    }

    inline unsigned long getCount() const {
        return count;
    }
};

#endif
