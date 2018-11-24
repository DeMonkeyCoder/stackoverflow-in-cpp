#pragma once

#include <iostream>
#include <vector>
#include "ContentRelation.h"

enum ContentType {
    QUESTION,
    ANSWER
};

class Content {
public:
    std::string body;
    ContentType type;
    int visits;
    std::vector<ContentRelation> relations;
};