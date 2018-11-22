#pragma once

#include <vector>
#include <string>
#include "User.h"

#define _Log(x) Logger::getInstance().log((x))


class Logger {
public:
    static Logger& getInstance();
public:
    void printLogs();
    void log(User& user);
    std::vector<std::string>& getLogs ();
private:
    Logger();
    std::vector<std::string> logs;
};
