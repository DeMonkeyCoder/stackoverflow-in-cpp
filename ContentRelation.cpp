#include "ContentRelation.h"

ContentRelation::ContentRelation(ContentRelationType &type, Content *destination):
    type(type), destination(destination) {}
