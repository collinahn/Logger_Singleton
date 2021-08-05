#ifndef __LOGGER__
#define __LOGGER__
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <ctime>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>

#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_FATAL 10
#define LOG_LEVEL_ERROR 20
#define LOG_LEVEL_WARN 30
#define LOG_LEVEL_INFO 40
#define LOG_LEVEL_DEBUG 50
#define LOG_LEVEL_TRACE 60
#define LOG_LEVEL_ALL 100

#define __LOG_PATH__ "/home/pi/log/"

#define FATAL(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_FATAL, str, __VA_ARGS__)
#define ERROR(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_ERROR, str, __VA_ARGS__)
#define WARN(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_WARN, str, __VA_ARGS__)
#define INFO(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_INFO, str, __VA_ARGS__)
#define DEBUG(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_DEBUG, str, __VA_ARGS__)
#define TRACE(str, ...) WriteLog(__FUNCTION__, __LINE__, LOG_LEVEL_TRACE, str, __VA_ARGS__)

using namespace std;

class Logger {
private:
    static Logger LoggerInstance;

    int mn_LogLevel;

    int GetDate();
    int GetMonth();
    int GetLastModifiedMonth(const char *sFilePath);

    std::string GetTimestamp();
    std::string GetFilePath();
    bool ArrangeLogFiles(std::string sFilePath);
    
public:
    static Logger *getInstance() { return &LoggerInstance; }    // 외부에서 Class를 Pointer 사용하기 위하여 사용

    Logger();
    Logger(int level);

    void WriteLog(const char *sFuncName, int nSourceLine, int nLv, const char *sLoggerMsg, ...);
};

#endif