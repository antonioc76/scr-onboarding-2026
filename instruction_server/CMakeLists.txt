cmake_minimum_required(VERSION 3.3)

project(InstructionServer)

add_subdirectory(messages)

add_executable(map src/team_map.c)

add_executable(server src/server.c)

add_executable(client src/client.c)

target_link_libraries(client PUBLIC my_lcmtypes)

target_link_libraries(server PUBLIC my_lcmtypes)
