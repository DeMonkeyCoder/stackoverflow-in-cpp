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
    std::vector<ContentRelation> relations;
    ContentType type;
    int visits;

public:
    Content(ContentType &type, std::string &body);
};
