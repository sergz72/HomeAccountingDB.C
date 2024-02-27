#ifndef H_SUBCATEGORIES_H
#define H_SUBCATEGORIES_H

#include "../core/object_array.h"

#define SUBCATEGORY_NAME_SIZE 80

enum SubcategoryOperationCode {
    incm,
    expn,
    spcl
};

enum SubcategoryCode {
    comb,
    comc,
    fuel,
    prcn,
    incc,
    expc,
    exch,
    trfr,
    none
};

struct Subcategory {
    unsigned long id;
    unsigned long category;
    char name[SUBCATEGORY_NAME_SIZE];
    SubcategoryCode code;
    SubcategoryOperationCode operationCode;
};

class Subcategories: public ObjectArray<Subcategory> {
public:
    inline explicit Subcategories(ObjectArraySource<Subcategory> *source, unsigned long capacity):
            ObjectArray<Subcategory>(source, capacity) {}
protected:
    bool isValid(const Subcategory *value) const override;
};

#endif
