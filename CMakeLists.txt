cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the source directory
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJ_NAME "csim")

project(${PROJ_NAME} VERSION 1.0)

# Find all .cpp files in the source directory
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
find_package(boost_program_options REQUIRED)

# Add the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Boost::program_options)
