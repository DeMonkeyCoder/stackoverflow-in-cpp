#include "ContentRelation.h"
#include "Content.h"

ContentRelation::ContentRelation(Content *destination, Content *source, ContentRelationType type):
    destination(destination),
    source(source),
    type(type) {}

ContentRelation::~ContentRelation() {
    auto relations = &(this->destination->relations);

    for(auto it = relations->begin(); it < relations->end(); it++) {
        if(*it == this) {
            relations->erase(it);
        }
    }

    relations = &(this->source->relations);

    for (auto it = relations->begin(); it < relations->end(); it++) {
        if (*it == this) {
            relations->erase(it);
        }
    }
}
