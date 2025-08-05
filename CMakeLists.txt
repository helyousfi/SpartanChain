cmake_minimum_required(VERSION 3.14)
project(SpartanChain)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(src)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(spartanchain ${SOURCES})

# Link OpenSSL libraries
target_link_libraries(spartanchain OpenSSL::Crypto)
 
