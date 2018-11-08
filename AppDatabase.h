//
// Created by spsina on 11/8/18.
//

#ifndef STACKOVERFLOW_IN_CPP1_APPDATABASE_H
#define STACKOVERFLOW_IN_CPP1_APPDATABASE_H

#include <vector>

template <class Type>
class AppDatabase {
public:
    std::vector<Type*> appDatas;
    AppDatabase(){}
};
#endif //STACKOVERFLOW_IN_CPP1_APPDATABASE_H
