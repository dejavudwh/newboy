#include <iostream>
#include "newboy/log.h"
#include "newboy/util.h"

int main(int argc, char** argv) {
    newboy::Logger::ptr logger(new newboy::Logger);
    logger->addAppender(newboy::LogAppender::ptr(new newboy::StdoutLogAppender));

    newboy::FileLogAppender::ptr file_appender(new newboy::FileLogAppender("./log.txt"));
    newboy::LogFormatter::ptr fmt(new newboy::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(newboy::LogLevel::ERROR);

    logger->addAppender(file_appender);

    std::cout << "hello newboy log" << std::endl;

    NEWBOY_LOG_INFO(logger) << "test macro";
    NEWBOY_LOG_ERROR(logger) << "test macro error";

    NEWBOY_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");

    auto l = newboy::LoggerMgr::GetInstance()->getLogger("xx");
    NEWBOY_LOG_INFO(l) << "xxx";
    return 0;
}