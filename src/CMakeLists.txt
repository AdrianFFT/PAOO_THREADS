cmake_minimum_required(VERSION 3.10)

project(ThreadsProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check for threading library and set appropriate flags
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(main.exe Threads.cpp)

# Link against the Threads::Threads library
target_link_libraries(main.exe PRIVATE Threads::Threads)

# Include any additional directories if needed
# target_include_directories(main.exe PUBLIC "${PROJECT_SOURCE_DIR}/include")

# If you want to link against external libraries, you can add them here
# target_link_libraries(main.exe PUBLIC SomeLibrary)

# Optionally, you can set compiler flags specific to this target
# target_compile_options(main.exe PRIVATE -Wall -Wextra -pedantic)
