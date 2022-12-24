cmake_minimum_required(VERSION 3.0)

# Set the project name
project(player)

# Enable testing
enable_testing()

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Set the include directories
include_directories(
    include
)

# Set the source files
set(SOURCES
    src/Player.cpp
)

# Set the header files
set(HEADERS
    include/Player.h
)

# Add the executable with the specified source and header files
add_executable(player src/main.cpp ${SOURCES} ${HEADERS})


# Link the GTest.Test library for testing
find_package(GTest REQUIRED)

# Set the source files for the unit tests
set(TEST_SOURCES
    test/test_Player.cpp
)

# Add the unit tests executable with the necessary source and header files
add_executable(unit_tests ${TEST_SOURCES} ${HEADERS} ${SOURCES})

# Link the Google Test libraries to the unit tests
target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES})

# Add the unit tests to be run by CTest
add_test(test_Player unit_tests)
