#ifndef H_LRU_H
#define H_LRU_H

template<typename T>
struct LruItem {
    T* data;
    unsigned long key;
    LruItem *prev;
    LruItem *next;

    LruItem(unsigned long _key, T* object, LruItem *_next) {
        key  = _key;
        data = object;
        prev = nullptr;
        next = _next;
    }

    explicit LruItem(unsigned long _key) {
        key  = _key;
        data = nullptr;
        prev = nullptr;
        next = nullptr;
    }
};

template<typename T>
class Lru {
    LruItem<T> *head;
    LruItem<T> *tail;

    void lruAttach(LruItem<T> *item) {
        if (head != nullptr)
            head->prev = item;
        else
            tail = item;
        head = item;
        activeItems++;
    }
protected:
    int activeItems;

    Lru() {
        head = tail = nullptr;
        activeItems = 0;
    }

    LruItem<T> *lruAdd(unsigned long key, T* object) {
        auto item = new LruItem<T>(key, object, head);
        lruAttach(item);
        return item;
    }

    void lruMoveToFront(LruItem<T> *item) {
        if (item == head)
            return;
        lruDetach(item);
        lruAttach(item);
    }

    void lruDetach(LruItem<T> *item) {
        activeItems--;
        if (item->prev != nullptr)
            item->prev->next = item->next;
        else
            head = item->next;
        if (item->next != nullptr)
            item->next->prev = item->prev;
        else
            tail = item->prev;
    }
};

#endif
