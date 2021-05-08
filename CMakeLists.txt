project(cpp-emarket)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(src SRCS)
include_directories(./include)

add_executable(main ${SRCS})
