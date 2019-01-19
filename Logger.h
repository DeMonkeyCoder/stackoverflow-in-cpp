//
// Created by SARM on 20/01/2019.
//

#ifndef STACKOVERFLOW_IN_CPP_LOGGER_H
#define STACKOVERFLOW_IN_CPP_LOGGER_H


#include <fstream>

class Logger {
    int logCount = 0;
    static Logger *instance;

    Logger() = default;

public:
    static Logger *getInstance();

    Logger operator<<(std::string s) const;
    void inc();
};


#endif //STACKOVERFLOW_IN_CPP_LOGGER_H
