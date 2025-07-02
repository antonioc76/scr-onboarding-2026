cmake_minimum_required(VERSION 3.3)

project(messages)

find_package(lcm REQUIRED)
include(${LCM_USE_FILE})

lcm_wrap_types(
    C_SOURCES c_sources
    C_HEADERS c_headers
    team_request_t.lcm
    team_response_t.lcm
)

lcm_add_library(my_lcmtypes C STATIC ${c_sources} ${c_headers})

target_include_directories(my_lcmtypes INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)