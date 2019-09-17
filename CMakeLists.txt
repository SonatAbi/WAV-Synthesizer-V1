cmake_minimum_required(VERSION 3.5)
project(SYNTH CXX)

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

add_executable(synth synth.cpp)
INSTALL(TARGETS synth DESTINATION bin)

enable_testing()
add_test(00_Install ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install)
add_subdirectory(tests)

