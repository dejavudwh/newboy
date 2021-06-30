#include "log.h"

namespace newboy {
    Logger::Logger(const std::string& name = "root")
        :m_name(name) {

    }
    void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
        if (level >= m_level) {
            for (auto& appender : m_appenders) {
                appender->log(level, event);
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
}