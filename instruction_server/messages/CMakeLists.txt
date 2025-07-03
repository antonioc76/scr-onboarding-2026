cmake_minimum_required(VERSION 3.3)

project(messages)

find_package(lcm REQUIRED)
find_package(Java REQUIRED)
include(${LCM_USE_FILE})

if(JAVA_FOUND)
  include(UseJava)
  set(java_args JAVA_SOURCES java_sources)
endif()

lcm_wrap_types(
    C_SOURCES c_sources
    C_HEADERS c_headers
    ${java_args}
    team_request_t.lcm
    team_response_t.lcm
)

if (JAVA_FOUND)
    add_jar(my_lcmtypes-jar
    OUTPUT_NAME my_lcmtypes
    INCLUDE_JARS lcm-java
    SOURCES ${java_sources}
  )
endif()

lcm_add_library(my_lcmtypes C STATIC ${c_sources} ${c_headers})

target_include_directories(my_lcmtypes INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)