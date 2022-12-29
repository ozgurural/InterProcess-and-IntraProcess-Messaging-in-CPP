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

