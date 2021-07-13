#include "../newboy/config.h"
#include <yaml-cpp/yaml.h>

newboy::ConfigVar<int>::ptr g_int_value_config = 
    newboy::Config::Lookup("system.port", (int)8080, "system port");

void test_yaml() {
    YAML::Node root = YAML::LoadFile("../bin/conf/log.yml");

    NEWBOY_LOG_INFO(NEWBOY_LOG_ROOT()) << root;
}

int main(int argc, char const *argv[])
{
    std::cout << "Hello Newboy Config!" << std::endl;
    std::cout << g_int_value_config->getValue() << g_int_value_config->toString() << std::endl;
    // getSS没有汇集所有流
    // NEWBOY_LOG_INFO(l) << g_int_value_config->getValue() << std::endl;
    // NEWBOY_LOG_INFO(l) << g_int_value_config->toString() << std::endl;
    
    test_yaml();
    
    return 0;
}
