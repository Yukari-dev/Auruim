cmake_minimum_required(VERSION 3.20)
project(Auruim VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE AURUIM_SOURCES
  src/auruim/*.cpp
)

add_library(Auruim STATIC ${AURUIM_SOURCES})

target_include_directories(Auruim
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/sandbox/main.cpp")
  add_executable(auruim sandbox/main.cpp)
  target_link_libraries(auruim PRIVATE Auruim)
endif()

install(TARGETS Auruim
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
