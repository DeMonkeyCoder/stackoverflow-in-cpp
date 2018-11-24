#pragma once

#include <iostream>
#include "Content.h"

enum ContentRelationType {
    DUPLICATE_OF,
    ANSWER_TO
};

class ContentRelation {
public:
    Content destination;
    ContentRelationType type;
};