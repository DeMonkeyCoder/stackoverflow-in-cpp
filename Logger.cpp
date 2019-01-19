//
// Created by SARM on 20/01/2019.
//

#include <sstream>
#include "Logger.h"

Logger *Logger::getInstance() {
    if (!instance)
        instance = new Logger();
    return instance;
}

Logger *Logger::instance = nullptr;

Logger Logger::operator<<(std::string s) const {
    char buf[100];
    std::stringstream ss(buf);
    ss << "log." << logCount << ".txt";
    std::string k = ss.str();
    std::fstream file(ss.str().c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
    file << s << " ";
    file.close();
    return *this;
}

void Logger::inc() {
    logCount++;
}


