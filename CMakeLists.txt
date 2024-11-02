cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# ----------------------------------------------------------------------------

project(csim)  # makes $PROJECT_NAME available

find_package(boost_program_options REQUIRED)
find_package(boost_timer REQUIRED)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(BOOST_LIBS Boost::program_options Boost::timer)
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBS})


# ----------------------------------------------------------------------------

include(FetchContent)
set(GT_HASH I63913152217c99b8c08ae6af8924bc28b02aa9d0)  # main/latest : Nov 1, 2024
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/${GT_HASH}.zip)

enable_testing()

find_package(GTest)
include(GoogleTest)

set(UT_NAME ut_suite)
set(GTEST_LIBS GTest::gtest_main)

add_executable(${UT_NAME} src/unittests.cc)
target_link_libraries(${UT_NAME} ${GTEST_LIBS})
gtest_discover_tests(${UT_NAME})
