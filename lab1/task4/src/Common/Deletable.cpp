#include "Deletable.h"

void Deletable::Delete() {
    deleted = true;
}

bool Deletable::IsDeleted() const {
    return deleted;
}

void Deletable::Remedy() {
    deleted = false;
}
