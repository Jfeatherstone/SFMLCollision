cmake_minimum_required(VERSION 3.13)

# Compile in c++ 17
set(CMAKE_CXX_STANDARD 17)

project(collision)

file(GLOB SRC src/*.cpp)

# Set our include directories
include_directories(include/)

# This imports the sfml libraries
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

#set(SOURCES ${SRC})

# Sets our main file
add_executable(${PROJECT_NAME} ${SRC})

# This allows us to use the sfml libraries
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system sfml-audio)
