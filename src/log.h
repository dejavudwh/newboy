#ifndef __NEWBOY_LOG_H__
#define __NEWBOY_LOG_H__

#include <string>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>

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
        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr formatter) {
            m_formatter = formatter;
        }
        LogFormatter::ptr getFormatter() {
            return m_formatter;
        }
    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
};

// 日志器
class Logger {
    public:
        typedef std::shared_ptr<Logger> ptr;

        Logger(const std::string& name = "root");
        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const {
            return m_level;
        };
        void setLevel(LogLevel::Level level) {
            m_level = level;
        };
    private:
        std::string m_name;                       // 日志名称
        LogLevel::Level m_level;                  // 日志级别（只会输出相应级别的日志）
        std::list<LogAppender::ptr> m_appenders;  // 
};

// stdout appender
class StdoutLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        void log(LogLevel::Level level, LogEvent::ptr event) override;
};

// file appender
class FileLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        
        FileLogAppender(const std::string& filename);
        void log(LogLevel::Level level, LogEvent::ptr event) override;

        // 重新打开文件； if success return true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
};

};

#endif