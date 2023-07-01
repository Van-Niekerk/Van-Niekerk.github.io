/**************************************************************************
	> File Name: logger.h
	> Author: 隔壁老孙无敌 
	> Mail:1686169484@qq.com 
	> Created Time: Sat Jun 17 20:11:41 2023
 ************************************************************************/

#ifndef _LOGGER_H
#define _LOGGER_H

#define LOG(level) MyLog::Logger::LoggerStream(level, __FILE__, __LINE__, MyLog::My_log)
#define LOG_INFO LOG(MyLog::LogLevel::INFO)
#define LOG_WARNING LOG(MyLog::LogLevel::WARNING)
#define LOG_DEBUG LOG(MyLog::LogLevel::DEBUG)
#define LOG_ERROR LOG(MyLog::LogLevel::ERROR)
#define LOG_FATAL LOG(MyLog::LogLevel::FATAL)
#define SET_LEVEL(level)  MyLog::My_log.set_level(level)

namespace MyLog {
    class LogLevel {
    public :
        static const int INFO;
        static const int WARNING;
        static const int DEBUG;
        static const int ERROR;
        static const int FATAL;
    };
    const int LogLevel::INFO = 1;
    const int LogLevel::WARNING = 2;
    const int LogLevel::DEBUG = 3;
    const int LogLevel::ERROR = 4;
    const int LogLevel::FATAL = 5;
    static std::map<int, std::string> LevelString = {
        {LogLevel::INFO, "INFO"},
        {LogLevel::WARNING, "WARNING"},
        {LogLevel::DEBUG, "DEBUG"},
        {LogLevel::ERROR, "ERROR"},
        {LogLevel::FATAL, "FATAL"},
    }; 
    class Logger {
    public :
        Logger() : LOG_LEVEL(LogLevel::INFO) {}
        class LoggerStream : public std::ostringstream {
        public :
            LoggerStream(int level,
            const char *file_name, int line_no, Logger &raw_log)
            : line_no(line_no), level(level), raw_log(raw_log) {
                std::ostringstream &now = *this;
                now << "[" << file_name << " : " << LevelString[level] << "]";
            }
            ~LoggerStream() {
                if (level < raw_log.LOG_LEVEL) return ;
                std::unique_lock<std::mutex> lock(raw_log.m_mutex);
                std::cout << this->str() << " (" << line_no << ")" << std::endl;
            }
        private:
            int line_no, level;
            Logger &raw_log;
            // 多线程中的互相不影响
        };
        void set_level(int level) { this->LOG_LEVEL = level; }
        int LOG_LEVEL;
        std::mutex m_mutex;
    };
    Logger My_log;
} // end of MyLog

#endif
