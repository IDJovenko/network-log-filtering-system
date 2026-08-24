cmake_minimum_required(VERSION 4.2)

project(ip-log-filtering
    VERSION 1.0.0
    LANGUAGES CXX
)

include(GNUInstallDirs)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_library(ip-log-filtering STATIC
    src/create_filter/create_filter.cpp
    src/filter/AndFilter.cpp
    src/filter/RangeFilter.cpp
    src/filter/SubnetFilter.cpp
    src/filter/factory/FilterFactory.cpp
    src/log_parser/LogParser.cpp
    src/process_stream/process_stream.cpp
    src/process_stream/output/BufferedLogWriter.cpp
    src/value_objects/IPv4.cpp
)

target_compile_features(ip-log-filtering PRIVATE cxx_std_23)

target_include_directories(ip-log-filtering PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_include_directories(ip-log-filtering PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filter
    ${CMAKE_CURRENT_SOURCE_DIR}/src/filter/factory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log_parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process_stream
    ${CMAKE_CURRENT_SOURCE_DIR}/src/process_stream/output
    ${CMAKE_CURRENT_SOURCE_DIR}/src/value_objects
)

include(cmake/install.cmake)
