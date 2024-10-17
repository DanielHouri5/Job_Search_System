cmake_minimum_required(VERSION 3.17)
project(JobSearchProject)

set(CMAKE_CXX_STANDARD 14)

# Add the SQLiteCpp directory to the build
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp)



# Add Google Test

# Define the executable for your main application
add_executable(
        untitled
        main.cpp
        authentication.cpp
        menus.cpp
        candidate.cpp
        employer.cpp
        TableCreation.cpp TableCreation.h)

# Link against SQLiteCpp (and sqlite3 if necessary)
target_link_libraries(untitled SQLiteCpp)

# Include directories for the main application
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/sqlite3)
