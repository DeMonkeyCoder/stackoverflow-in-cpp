#include "Content.h"

Content::Content(std::string &body, ContentType &type): visits(0), type(type), body(body) {}

Content::~Content() {
    for(int i = 0; i < relations.size(); i++)
        delete relations[0];
}

void Content::add_relation(ContentRelationType &type, Content &destination) {
    auto relation_ptr = new ContentRelation(&destination, this,  type);
    relations.push_back(relation_ptr);
    destination.relations.push_back(relation_ptr);
}

void Content::edit_content(std::string &body) {
    this->body = body;
}

void Content::print_answers() {
    int answer_number = 1;
    for(int i = 0; i < relations.size(); i++) {
        if(relations[i]->type == ANSWER_TO) {
            std::cout << answer_number++ << ". " << relations[i]->source->body << std::endl;
        }
    }
    std::cout << std::endl;
}
