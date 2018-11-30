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
    std::vector<ContentRelation*> relations;

public:
    Content(std::string &body, ContentType &type);
    ~Content();
    void add_relation(ContentRelationType &type, Content &destination);
    void edit_content(std::string &body);
    void print_answers();
};
