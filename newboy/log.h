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
#include <map>
#include <functional>
#include <cstdarg>

namespace newboy {

class Logger;

// 日志事件
class LogEvent {
    public:
        typedef std::shared_ptr<LogEvent> ptr;

        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
                 const char* file, int32_t line, uint32_t elapse,
                 uint32_t thread_id, uint32_t fiber_id,
                 uint64_t time ,const std::string& thread_name);

        const char* getFile() { return m_file; }
        int32_t getLine() { return m_line; }
        uint32_t getElapse() { return m_elapse; }
        uint32_t getThreadId() { return m_threadId; }
        uint32_t getFiberId() { return m_fiberId; }
        uint64_t getTime() { return m_time; }
        std::string getContent() const { return m_ss.str(); }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }
        LogLevel::Level getLevel() const { return m_level; }
        std::stringstream& getSS() { return m_ss;}
        const std::string& getThreadName() const { return m_threadName;}

        void format(const char* fmt, ...);
        void format(const char* fmt, va_list al);
    private:
        const char* m_file = nullptr;   // 文件名
        int32_t m_line = 0;             // 行号
        uint32_t m_elapse = 0;          // 程序启动到现在的毫秒数
        std::string m_threadName;
        uint32_t m_threadId = 0;        // 线程id
        uint32_t m_fiberId = 0;         // 协程id
        uint64_t m_time = 0;            // 时间戳
        std::stringstream m_ss;         // 日志流
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
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

    static const char* toString(LogLevel::Level level);
};

// 日志格式化器
class LogFormatter {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;

        LogFormatter(const std::string& pattern);
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
        std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    
        void init();
        bool isError() const { return m_error; }
        const std::string getPattern() const { return m_pattern; }

    public:
        class FormatItem {
            public:
                typedef std::shared_ptr<FormatItem> ptr;

                virtual ~FormatItem();
                virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };
    
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
        bool m_error = false;
};

// 日志输出处
class LogAppender {
    friend class Logger;
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
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }

    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        bool m_hasFormatter = false;
        LogFormatter::ptr m_formatter;
};

// 日志器
class Logger : public std::enable_shared_from_this<Logger>{
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
        void clearAppenders();

        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level level) { m_level = level; }

        const std::string& getname() const { return m_name; }
        void setFormatter(LogFormatter::ptr val);
        void setFormatter(const std::string& val);
        LogFormatter::ptr getFormatter();
        
    protected:
        std::string m_name;                       // 日志名称
        LogLevel::Level m_level;                  // 日志级别（只会输出相应级别的日志）
        std::list<LogAppender::ptr> m_appenders;  // 
        LogFormatter::ptr m_formatter;
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
        uint64_t m_lastTime = 0;
};

};

#endif