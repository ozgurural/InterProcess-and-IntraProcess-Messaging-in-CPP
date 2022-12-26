cmake_minimum_required(VERSION 3.22)

# Set the project name
project(player)

# Enable testing
enable_testing()

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)

# Set the include directories
include_directories(
    include
)

# Set the source files
set(SOURCES
    src/Player.cpp
    src/Logger.cpp
    src/PlayerManager.cpp
)

# Set the header files
set(HEADERS
    include/Player.h
    include/Logger.h
    include/PlayerManager.h
)

# Add the executable with the specified source and header files
add_executable(player src/main.cpp ${SOURCES} ${HEADERS})


# Link the GTest.Test library for testing
find_package(GTest REQUIRED)

# Set the source files for the unit tests
set(TEST_SOURCES
    test/test_Player.cpp
    test/test_Logger.cpp
    #test/test_PlayerManager.cpp
)

# Add the unit tests executable with the necessary source and header files
add_executable(unit_tests ${TEST_SOURCES} ${HEADERS} ${SOURCES})

# Link the Google Test libraries to the unit tests
target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES})

# Add the unit tests to be run by CTest
add_test(test_Player unit_tests)
add_test(test_Logger unit_tests)
#add_test(test_PlayerManager unit_tests)
