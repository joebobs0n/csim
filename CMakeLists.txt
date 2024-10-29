cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PROJ_NAME "csim")
project(${PROJ_NAME} VERSION 1.0)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
find_package(boost_program_options REQUIRED)
find_package(boost_timer REQUIRED)
set(BOOST_LIBS Boost::program_options Boost::timer)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBS})
