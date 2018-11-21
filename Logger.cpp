#include "Logger.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

Logger::Logger() {}

Logger& Logger::getInstance() {
    static Logger lg;
    return lg;
}

void Logger::log(const std::string& lg) {
    std::ofstream file("Log.txt", std::ios::app);
    time_t now = time(0);
    std::string date_time(ctime(&now));
    file << "┌─[" << date_time << "└─[" << lg << std::endl;
    this->logs.push_back("┌─[" + date_time + "└─[" + lg);
}

void Logger::printLogs() {
    for(auto &log : logs) {
        std::cout << log << std::endl;
    }
}

std::vector<std::string>& Logger::getLogs() {
    return this->logs;
}
