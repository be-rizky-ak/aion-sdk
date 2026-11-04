cmake_minimum_required(VERSION 3.16)
project(AionBasicApplication CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the installed AionEngine package
find_package(AionEngine CONFIG REQUIRED)

# Basic application executable
add_executable(basic-application main.cpp)

# Link against the public AionEngine target
target_link_libraries(basic-application PRIVATE Aion::AionEngine)
