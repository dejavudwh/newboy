#include <iostream>
#include <string>
#include "../newboy/log.h"

int main(int argc, char const *argv[]) {
    std::cout << "Hello Newboy Logger!" << std::endl;

    newboy::Logger::ptr logger(new newboy::Logger);
    newboy::LogFormatter::ptr fmt(new newboy::LogFormatter("%d%T%p%T%c%T%f%T%l%n"));

    newboy::StdoutLogAppender::ptr stdout_appender(new newboy::StdoutLogAppender);
    stdout_appender->setFormatter(fmt);
    logger->addAppender(stdout_appender);

    newboy::FileLogAppender::ptr file_appender(new newboy::FileLogAppender("./log.txt"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(newboy::LogLevel::ERROR);

    logger->addAppender(file_appender);

    newboy::LogEvent::ptr event(new newboy::LogEvent(logger, newboy::LogLevel::DEBUG, 
                                                     __FILE__, __LINE__, 0, rand(), rand(),
                                                     time(0), std::string("test_log")));
    // event->getSS() << "hello sylar log";
    logger->log(newboy::LogLevel::DEBUG, event);

    std::cout << "\nHello Newboy Logger for macro!" << std::endl;

    NEWBOY_LOG_INFO(logger) << "test macro\n";
    NEWBOY_LOG_ERROR(logger) << "test macro error\n";

    NEWBOY_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    std::cout << "\nHello Newboy Logger for LoggerMgr!" << std::endl;
    auto l = newboy::LoggerMgr::GetInstance()->getLogger("xx");
    newboy::StdoutLogAppender::ptr lm_std_appender(new newboy::StdoutLogAppender);
    // TODO: logger自己的format没传下去
    lm_std_appender->setFormatter(fmt);
    l->addAppender(lm_std_appender);
    NEWBOY_LOG_INFO(l) << "xxx";

    return 0;
}
