#ifndef _TL_LOGGER_H
#define _TL_LOGGER_H
#include "TLCommonBase.h"

#include <string>
#include <ctime>
#include <mutex>
#include <fstream>

namespace tl{
    #define FILE_STACK std::string(__FILE__) + std::string(":[") + std::to_string(__LINE__) + std::string("]:(")  + std::string(__FUNCTION__) + std::string("):")

    enum LogLevel
    {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    class CLogAppender
    {
    public:
        virtual void append(const std::string& message) = 0;
    };

    //控制台输出
    class TL_COMMON_API_ CConsoleAppender : public CLogAppender
    {
    public:
        void append(const std::string& message);
    };

    //日志输出到文件
    class TL_COMMON_API_ CFileAppender : public CLogAppender
    {
    public:
        CFileAppender(const std::string& filename);

        ~CFileAppender();

    public:
        void append(const std::string& message);

    private:
        std::ofstream* pfile_;
        std::mutex* pmutex_;
    };

    //日志记录操作
    class TL_COMMON_API_ CLogger
    {
    public:
        CLogger(LogLevel level, CLogAppender* appender);

        //错误日志单独的文件
        CLogger(LogLevel level, CLogAppender* appender, CLogAppender* errorAppender);

        void debug(const std::string& message);

        void info(const std::string& message);

        void warn(const std::string& message);

        void error(const std::string& message);

        void fatal(const std::string& message);

        void setLevel(LogLevel level);

        LogLevel getLevel() const;

    private:
        void log(LogLevel level, const std::string& message);

    private:
        LogLevel level_;
        CLogAppender* appender_;
        CLogAppender* errorAppender_;
    };
}

#endif