cmake_minimum_required(VERSION 3.5)

project(cpp-emarket)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./include)

add_subdirectory(src)
add_library(sockpp STATIC $<TARGET_OBJECTS:sockpp-objs>)

add_executable(client $<TARGET_OBJECTS:client-obj> $<TARGET_OBJECTS:datatype>)
add_executable(server $<TARGET_OBJECTS:server-obj> $<TARGET_OBJECTS:datatype>)

target_link_libraries(client sockpp)
target_link_libraries(server sockpp Threads::Threads)

configure_file(test.sh test.sh)

add_custom_target(
	test
	COMMAND ./test.sh
	DEPENDS client server
	)
