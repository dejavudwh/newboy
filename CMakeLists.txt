cmake_minimum_required(VERSION 2.8)
project(newboy)

set(CMAKE_VERBOSE_MAKEFILE ON)
# $ENV{CXXFLAGS} 表示C++编译选项
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(.)
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

# find_library(yaml_cpp libyaml-cpp ${PROJECT_SOURCE_DIR}/lib)

set(LIB_SRC
    newboy/log.cc
    newboy/util.cc
    newboy/config.cc
    newboy/env.cc
    )

add_library(newboy SHARED ${LIB_SRC})

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log newboy)
target_link_libraries(test_log
    newboy
    yaml-cpp)

# add_executable(test_config tests/test_config.cc)
# add_dependencies(test_config newboy)
# target_link_libraries(test_config
#     newboy
#     yaml-cpp
#     )

