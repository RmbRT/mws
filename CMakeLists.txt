cmake_minimum_required(VERSION 2.6)
project(mws)

# Set the C++ standard used by the miasma web server.
set(CMAKE_CXX_FLAGS "-std=c++11 -Wfatal-errors")

# Select all source files.
file(GLOB_RECURSE mws_sources ./src/*.cpp)

# Add them to the mws library.
add_library(mws STATIC ${mws_sources})

# Add include files.
include_directories(include/)

# Add the netlib library.
add_subdirectory(depend/netlib)

# Link mws and netlib
target_link_libraries(mws netlib)

# Creates an include directory containing all header files used in the miasma web server.
# Add mws/include/ to your include directories and access the files via #include <mws/*>
file(COPY "src/" DESTINATION "include/mws/" FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl")
file(COPY "depend/netlib/include/" DESTINATION "include/")