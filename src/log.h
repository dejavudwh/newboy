#ifndef __NEWBOY_LOG_H__
#define __NEWBOY_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>

namespace newboy {

// 日志事件
class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent();
    
    private:
        const char* m_file = nullptr;   // 文件名
        int32_t m_line = 0;             // 行号
        uint32_t m_threadId = 0;        // 线程id
        uint32_t m_fiberId = 0;         // 协程id
        uint64_t m_time = 0;            // 时间戳
        std::string m_content;
};

// 日志事件级别
class LogLevel {
    public:
        enum Level {
            DEBUG = 1,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };
};

// 日志格式化器
class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        std::string format(LogEvent::ptr event);
    
    private:
};

// 日志输出处
class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender();
        void Log(LogLevel::Level level, LogEvent::ptr event);
    private:
        LogLevel::Level m_level;
};

// 日志器
class Logger {
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string& name = "root");

    private:
        std::string m_name;
        LogLevel::Level m_level;
        LogAppender::ptr m_appender;
};

// stdout appender
class StdoutLogAppender : public LogAppender {

};

// file appender
class FileLogAppender : public LogAppender {

};

};

#endif