#pragma once

#include <vector>
#include <string>

#define _Log(x) Logger::getInstance().log((x))


class Logger {
public:
    static Logger& getInstance();
public:
    void printLogs();
    void log(std::string lg);
    std::vector<std::string>& getLogs ();
private:
    Logger();
    std::vector<std::string> logs;
};
