#include <iostream>
#include <string>
#include "newboy/log.h"

int main(int argc, char const *argv[]) {
    std::cout << "Hello Newboy Logger!" << std::endl;

    newboy::Logger::ptr logger(new newboy::Logger);

    newboy::LogEvent::ptr event(new newboy::LogEvent(logger, newboy::LogLevel::DEBUG, 
                                                     __FILE__, __LINE__, 0, rand(), rand(),
                                                     time(0), std::string("test_log")));
    event->getSS() << "hello sylar log";
    logger->log(newboy::LogLevel::DEBUG, event);

    return 0;
}
