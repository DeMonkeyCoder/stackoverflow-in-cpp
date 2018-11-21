#include "Logger.h"
#include <iostream>
#include <vector>

Logger::Logger() {}

Logger& Logger::getInstance() {
    static Logger lg;
    return lg;
}

void Logger::log(std::string lg) {
    this->logs.push_back(lg);
}

void Logger::printLogs() {
    for(auto &log : logs) {
        std::cout << log << std::endl;
    }
}

std::vector<std::string>& Logger::getLogs() {
    return this->logs;
}
