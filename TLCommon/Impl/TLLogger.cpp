#include "../Export/TLLogger.h"
#include <iostream>
namespace tl {

    void CConsoleAppender::append(const std::string& message)
    {
        std::cout << message.c_str() << std::endl;
    }

    CFileAppender::CFileAppender(const std::string& filename)
    {
        pfile_ = new std::ofstream();
        pmutex_ = new std::mutex();
        pfile_->open(filename.c_str(), std::ios_base::out | std::ios_base::app);
    }

    void CFileAppender::append(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(*pmutex_);
        (*pfile_) << message.c_str() << std::endl;
    }

    CFileAppender::~CFileAppender()
    {
        pfile_->close();
        delete pfile_;
        pfile_ = nullptr;
        delete pmutex_;
        pmutex_ = nullptr;
    }

    CLogger::CLogger(LogLevel level, CLogAppender* appender)
        : level_(level), appender_(appender)
    {
        errorAppender_ = nullptr;
    }

    CLogger::CLogger(LogLevel level, CLogAppender* appender, CLogAppender* errorAppender)
        : level_(level), appender_(appender), errorAppender_(errorAppender)
    {

    }

    void CLogger::debug(const std::string& message)
    {
        log(DEBUG, message);
    }

    void CLogger::info(const std::string& message)
    {
        log(INFO, message);
    }

    void CLogger::warn(const std::string& message)
    {
        log(WARN, message);
    }

    void CLogger::error(const std::string& message)
    {
        log(ERROR, message);
    }

    void CLogger::fatal(const std::string& message)
    {
        log(FATAL, message);
    }

    void CLogger::setLevel(LogLevel level)
    {
        level_ = level;
    }

    LogLevel CLogger::getLevel() const
    {
        return level_;
    }

    void CLogger::log(LogLevel level, const std::string& message)
    {
        if (level >= level_)
        {
            time_t rawtime;
            struct tm timeinfo;
            char timestr[80];

            time(&rawtime);
            localtime_s(&timeinfo, &rawtime);

            strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", &timeinfo);

            std::string levelstr;
            switch (level)
            {
            case DEBUG: levelstr = "DEBUG"; break;
            case INFO: levelstr = "INFO"; break;
            case WARN: levelstr = "WARN"; break;
            case ERROR: levelstr = "ERROR"; break;
            case FATAL: levelstr = "FATAL"; break;
            }

            std::string output = "[" + std::string(timestr) + "] [" + levelstr + "] " + message;
            if (ERROR == level && this->errorAppender_)
            {
                this->errorAppender_->append(output);
            }
            else
            {
                appender_->append(output);
            }
        }
    }
}