#pragma once

#include <iostream>
#include<vector>

enum ContentRelationType {
    DUPLICATE_OF,
    ANSWER_TO
};

class Content;

class ContentRelation {
public:
    ContentRelation(Content* destination, Content* source, ContentRelationType type);
    ~ContentRelation();

public:
    Content* destination;
    Content* source ;
    ContentRelationType type;
};
