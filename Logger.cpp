#include "Logger.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

Logger::Logger() {}

Logger& Logger::getInstance() {
    static Logger lg;
    return lg;
}
extern int log_num;

void Logger::log(User& user) {
    std::ofstream log_file("log." + std::to_string(log_num) + ".txt", std::ios::app);
    time_t now = time(0);
    std::string date_time(ctime(&now));
    log_file << user << " | " << date_time;
    this->logs.push_back(user.toString() + " | " + date_time);
}

void Logger::printLogs() {
    for(auto &log : logs) {
        std::cout << log << std::endl;
    }
}

std::vector<std::string>& Logger::getLogs() {
    return this->logs;
}
