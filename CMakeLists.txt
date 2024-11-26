# cmake top level directives
cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ----------------------------------------------------------------------------

# set up global sources library for main and unit tests
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*/*.cpp")
file(GLOB INCLUDES "${CMAKE_SOURCE_DIR}/include/*")

add_library(global_sources STATIC ${SOURCES})
target_include_directories(global_sources PUBLIC ${INCLUDES})

# ----------------------------------------------------------------------------

# main executable build parameters
project(csim)

find_package(boost_program_options REQUIRED)
find_package(boost_timer REQUIRED)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE global_sources)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options Boost::timer)

# ----------------------------------------------------------------------------

# unit testing executable build parameters
include(FetchContent)
# set(GT_HASH "I63913152217c99b8c08ae6af8924bc28b02aa9d0")  # latest as of November 1, 2024
set(GT_HASH "I786e5b50d171c697c21579659a67716442f224df")  # latest as of November 20, 2024
FetchContent_Declare(googletest URL "https://github.com/google/googletest/archive/${GT_HASH}.zip")

enable_testing()

find_package(GTest)
include(GoogleTest)
set(GTEST_LIBS GTest::gtest_main)

set(TEST_SOURCES "planarfet_model.cc")
foreach(TEST_SOURCE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    set(UT_NAME "ut_${TEST_NAME}")
    add_executable(${UT_NAME} ${CMAKE_SOURCE_DIR}/tests/${TEST_SOURCE})
    target_link_libraries(${UT_NAME} PRIVATE global_sources GTest::gtest_main)
    gtest_discover_tests(${UT_NAME})
endforeach()
