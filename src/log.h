#ifndef __NEWBOY_LOG_H__
#define __NEWBOY_LOG_H__

#include <string>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>

namespace newboy {

class Logger;

// 日志事件
class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent();

        const char* getFile() { return m_file; }
        int32_t getLine() { return m_line; }
        uint32_t getElapse() { return m_elapse; }
        uint32_t getThreadId() { return m_threadId; }
        uint32_t getFiberId() { return m_fiberId; }
        uint64_t getTime() { return m_time; }
        const std::string& getContent() const { return m_content; }
    private:
        const char* m_file = nullptr;   // 文件名
        int32_t m_line = 0;             // 行号
        uint32_t m_elapse = 0;          // 程序启动到现在的毫秒数
        uint32_t m_threadId = 0;        // 线程id
        uint32_t m_fiberId = 0;         // 协程id
        uint64_t m_time = 0;            // 时间戳
        std::string m_content;
};

// 日志事件级别
class LogLevel {
    public:
        enum Level {
            UNKOWN = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
        };

    static const char* ToString(LogLevel::Level level);
};

// 日志格式化器
class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        LogFormatter(const std::string& pattern);
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    
    private:
        class FormatItem {
            public:
                typedef std::shared_ptr<FormatItem> ptr;

                virtual ~FormatItem();
                virtual void format(std::ostream os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };
    
        void init();
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
};

// 日志输出处
class LogAppender {
    public:
        typedef std::shared_ptr<LogAppender> ptr;

        virtual ~LogAppender();
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

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

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level level) { m_level = level; }

        const std::string& getname() const { return m_name; }
    private:
        std::string m_name;                       // 日志名称
        LogLevel::Level m_level;                  // 日志级别（只会输出相应级别的日志）
        std::list<LogAppender::ptr> m_appenders;  // 
};

// stdout appender
class StdoutLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;

        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
};

// file appender
class FileLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        
        FileLogAppender(const std::string& filename);
        void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;

        // 重新打开文件； if success return true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
};

};

#endif