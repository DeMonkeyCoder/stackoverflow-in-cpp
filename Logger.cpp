#include "Logger.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

Logger::Logger() {
    std::ifstream file;
    int num = 1;
    while(true) {
        file.open("log." + to_string(num) + ".txt");
        if(file.is_open()) {
            file.close();
            num++;
            continue;
        }
        log_num = num;
        std::ofstream log_file("log." + std::to_string(log_num) + ".txt");
        break;
    }
}

Logger& Logger::getInstance() {
    static Logger lg;
    return lg;
}

void Logger::log(User& user) {
    std::ofstream log_file("log." + std::to_string(Logger::getInstance().log_num) + ".txt", std::ios::app);
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
