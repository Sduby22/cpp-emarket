cmake_minimum_required(VERSION 3.5)

project(cpp-emarket)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./include)

# --- System libraries ---

include(GNUInstallDirs)

option(BUILD_DOC "Build Doxygen" OFF)
find_package(
  Doxygen
  OPTIONAL_COMPONENTS mscgen dia dot
)

if(BUILD_DOC AND DOXYGEN_FOUND)
  add_subdirectory(doc)
else()
  message("Needs doxygen installed to generate documentation")
endif()

if(WIN32)
  set(SQLite3_INCLUDE_DIRS ./sqlite3/include)
  set(SQLite3_LIBRARY ./sqlite3/lib)
  set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
  set(LIBS_SYSTEM c stdc++)
endif()

find_package(SQLite3 REQUIRED)

add_subdirectory(src)
add_library(sockpp STATIC $<TARGET_OBJECTS:sockpp-objs>)
target_link_libraries(sockpp ${LIBS_SYSTEM})

add_executable(client $<TARGET_OBJECTS:client-obj> $<TARGET_OBJECTS:datatype>)
add_executable(server $<TARGET_OBJECTS:server-obj> $<TARGET_OBJECTS:datatype>)

target_link_libraries(client sockpp)
target_link_libraries(server sockpp Threads::Threads sqlite3)

configure_file(test.sh test.sh)

add_custom_target(
  test
  COMMAND ./test.sh
  DEPENDS client server
)
