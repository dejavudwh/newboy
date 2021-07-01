#include "log.h"

namespace newboy {

/**
 * Logger
 **/
Logger::Logger(const std::string& name = "root")
    :m_name(name) {}
void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        for (auto& appender : m_appenders) {
            appender->log(this, level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    // debug(LogLevel::DEBUG, event);
}
void Logger::info(LogEvent::ptr event) {

}
void Logger::warn(LogEvent::ptr event) {

}
void Logger::error(LogEvent::ptr event) {

}
void Logger::fatal(LogEvent::ptr event) {

}

void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin();
                it != m_appenders.end();
                ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
        }
    }
}

/**
 * Appender
 **/

FileLogAppender::FileLogAppender(const std::string& filename)
    :m_filename(filename) {}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }

    m_filestream.open(m_filename);
    
    return !!m_filestream;
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if (level > m_level) {
        std::cout << m_formatter->format(logger, level, event);
    }
}

/**
 * LogLevel
 **/

const char* LogLevel::ToString(LogLevel::Level level) {
    switch (level) {
        #define XX(name) \
            case LogLevel::name: \
                return #name; \
                break;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);

        #undef XX

        default:
            return "UNKOWN";
    }

    return "UNKOWN";
}

/**
 * Formatter
 **/

LogFormatter::LogFormatter(const std::string& pattern)
    :m_pattern(pattern) {}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto& i : m_items) {
        i->format(ss, logger, level, event);
    }

    return ss.str();
}

}