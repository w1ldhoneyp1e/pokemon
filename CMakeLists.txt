cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(Pokemon)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")   
find_package(SFML 2.6 COMPONENTS system window graphics audio network REQUIRED)

add_executable(Pokemon main.cpp)
target_link_libraries(Pokemon sfml-system sfml-window sfml-graphics sfml-audio)