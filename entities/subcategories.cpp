#include "subcategories.h"
#include <cstring>

bool Subcategories::isValid(const Subcategory *value) const {
    return value->id > 0;
}
