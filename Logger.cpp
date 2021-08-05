// 기능: 1) 일별 로그 파일을 생성한다
//          저장 위치: /home/pi/log
//          파일 이름: 날짜.log (예. 16일 -> 16.log)
//          월별 기록은 하지 않는다.(파일 있는지 확인하고, 이름이 같고 작성한 달이 다르면 삭제함)
//       2) 실행 중에 외부에서 로그레벨을 바꾸어 기록되는 것을 결정할 수 있도록 한다.
//
//
// 이력: 1) 2021.06.16 created by taeyoung
//       2) 2021.06.17 modifed by taeyoung: 재컴파일 하지 않아도 로그레벨 바꾸어 기록할 수 있도록 추가
//

#include "Logger.h"

#ifndef _makeExe_
Logger Logger::LoggerInstance;
#endif

Logger::Logger() {
    this->mn_LogLevel = LOG_LEVEL_INFO;
}

Logger::Logger(int nLogLevel) {
    this->mn_LogLevel = nLogLevel;
}

string Logger::GetTimestamp() {
    string s_Timestamp;
    time_t t_CurrentTime = time(NULL);
    tm *st_Time = localtime(&t_CurrentTime);

    ostringstream oss_TimeStream;
    switch(st_Time->tm_mon) {
        case(0): s_Timestamp = "Jan"; break;
        case(1): s_Timestamp = "Feb"; break;
        case(2): s_Timestamp = "Mar"; break;
        case(3): s_Timestamp = "Apr"; break;
        case(4): s_Timestamp = "May"; break;
        case(5): s_Timestamp = "Jun"; break;
        case(6): s_Timestamp = "Jul"; break;
        case(7): s_Timestamp = "Aug"; break;
        case(8): s_Timestamp = "Sep"; break;
        case(9): s_Timestamp = "Oct"; break;
        case(10): s_Timestamp = "Nov"; break;
        case(11): s_Timestamp = "Dec"; break;
    }
    oss_TimeStream.clear();
    oss_TimeStream << " " << setfill('0') << setw(2) << st_Time->tm_mday << " " << st_Time->tm_year+1900;
    oss_TimeStream << " " << setfill('0') << setw(2) << st_Time->tm_hour;
    oss_TimeStream << ":" << setfill('0') << setw(2) << st_Time->tm_min;
    oss_TimeStream << ":" << setfill('0') << setw(2) << st_Time->tm_sec << '\0';
    s_Timestamp = s_Timestamp + oss_TimeStream.str();
    return s_Timestamp;
}

int Logger::GetDate() {
    time_t t_CurrentTime = time(NULL);
    tm *st_Time = localtime(&t_CurrentTime);

    return st_Time->tm_mday;
}

int Logger::GetMonth() {
    time_t t_CurrentTime = time(NULL);
    tm *st_Time = localtime(&t_CurrentTime);

    return st_Time->tm_mon + 1;         // 1~12
}

//파일로부터 마지막으로 수정한 달 정보를 얻어온다
int Logger::GetLastModifiedMonth(const char *sFilePath) {
    struct stat st_Stat;

    if (stat(sFilePath, &st_Stat) < 0) {
        fprintf(stderr, "Logger::LastModifiedMonth FileStatError\n");
        return -1;
    }

    tm *st_Time = localtime(&st_Stat.st_mtim.tv_sec);

    return st_Time->tm_mon + 1;         // 1~12
}

//일별로 변하는 FilePath를 반환한다.
//저장 위치: /home/pi/log
//ex) "/home/pi/log/17.log" (17일 생성된 로그 파일)
std::string Logger::GetFilePath() {
    ostringstream oss_PathStream;

    oss_PathStream.clear();
    oss_PathStream << __LOG_PATH__ << GetDate() << ".log";

    return oss_PathStream.str();
}

//이름이 같은 파일이 있으면 삭제한다.
bool Logger::ArrangeLogFiles(std::string sFilePath) {
    try {
        int n_Ret;

        if (access(sFilePath.c_str(), F_OK) == 0) {                         //있는지 확인한다.
            int n_MonthNow = GetMonth();
            int n_MonthBefore = GetLastModifiedMonth(sFilePath.c_str());
            if (n_MonthBefore == -1) return false;

            //Last modified정보를 확인하여 같은 달에 생성된 것이면 지우지 않는다.
            if (n_MonthNow == n_MonthBefore) return true;

            n_Ret = remove(sFilePath.c_str());
            if (n_Ret == 0) return true;
            return false;

        } else if (access(__LOG_PATH__, F_OK) == 0) {                       //상위 폴더 있는지 확인한다.
            return true;
        } else {                                                            //폴더가 없는 경우
            mkdir(__LOG_PATH__, 0777);
            return true;
        }
    } catch(exception& e) {
        return false;
    }
}

// 로그 파일 출력하는 함수
// [FATAL] Jun 16 2021 20:58:48 [main:89] : sLoggerMsg1
// [ERROR] Jun 16 2021 20:58:48 [main:90] : sLoggerMsg2
// [WARN]  Jun 16 2021 20:58:48 [main:91] : sLoggerMsg3
// [INFO]  Jun 16 2021 20:58:48 [main:92] : sLoggerMsg4
// [DEBUG] Jun 16 2021 20:58:48 [main:93] : sLoggerMsg5
// [TRACE] Jun 16 2021 20:58:48 [main:94] : sLoggerMsg6
void Logger::WriteLog(const char *sFuncName, int nSourceLine, int nLv, const char *sLoggerMsg, ...) {
    FILE *fp = NULL;
    std::string s_FilePath = GetFilePath();
    bool b_Ret = ArrangeLogFiles(s_FilePath);
    if (b_Ret == false) return;

    fp = fopen(s_FilePath.c_str(), "a");
    if(fp == NULL) {
        fprintf(stderr, "Logger::WriteLog FileOpenError\n");
        return;
    }
    char *s_MsgWrite2File = NULL;
    char s_LogLevel[10];
        
    switch(nLv) {
        case(LOG_LEVEL_FATAL): strcpy(s_LogLevel, "[FATAL]"); break;
        case(LOG_LEVEL_ERROR): strcpy(s_LogLevel, "[ERROR]"); break;
        case(LOG_LEVEL_WARN): strcpy(s_LogLevel, "[WARN] "); break;
        case(LOG_LEVEL_INFO): strcpy(s_LogLevel, "[INFO] "); break;
        case(LOG_LEVEL_DEBUG): strcpy(s_LogLevel, "[DEBUG]"); break;
        case(LOG_LEVEL_TRACE): strcpy(s_LogLevel, "[TRACE]"); break;
    }
    
    s_MsgWrite2File = (char*)malloc(sizeof(char)*(21+strlen(sFuncName)+strlen(sLoggerMsg)+30));
    if(strchr(sLoggerMsg, '\n') == nullptr) {
        sprintf(s_MsgWrite2File, "%s %s [%s:%d] : %s\n", s_LogLevel, GetTimestamp().c_str(), sFuncName, nSourceLine, sLoggerMsg);
    } else {
        sprintf(s_MsgWrite2File, "%s %s [%s:%d] : %s", s_LogLevel, GetTimestamp().c_str(), sFuncName, nSourceLine, sLoggerMsg);
    }

    va_list va_Args;
    va_start(va_Args, sLoggerMsg);
    vfprintf(fp, s_MsgWrite2File, va_Args);
    va_end(va_Args);

    //콘솔에 출력한다.
    #ifdef _PRINT_CONSOLE_
    va_start(va_Args, sLoggerMsg);
    if(this->n_LogLevel >= nLv) {
        vprintf(s_MsgWrite2File, va_Args);
    }
    va_end(va_Args);
    #endif

    if(s_MsgWrite2File != NULL) {
        free(s_MsgWrite2File);
    }
    if(fp != NULL) {
        fclose(fp);
    }
    return;
}

#ifdef _makeExe_
int main()
{
    std::string s = "Logger Class in Singleton";
    Logger test(LOG_LEVEL_WARN);
    test.FATAL("fatal", 1);
    test.ERROR("error", 1);
    test.WARN(s.c_str(), 1);
    test.INFO("info", 1);
    test.DEBUG("debug", 1);
    test.TRACE("trace", 1);
    return 0;
}
#else
extern "C" {
    Logger* Logger_new() { return new Logger(); }
}
#endif