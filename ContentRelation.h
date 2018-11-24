#pragma once

#include <iostream>

enum ContentRelationType {
    DUPLICATE_OF,
    ANSWER_TO
};

class Content;

class ContentRelation {
public:
    Content* destination;
    ContentRelationType type;
};