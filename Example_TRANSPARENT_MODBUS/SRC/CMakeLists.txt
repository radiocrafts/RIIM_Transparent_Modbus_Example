cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE ../../../Framework/Tools/Toolchain.txt)

get_filename_component(PROJECT_NAME ${SOURCE_FILE} NAME_WE)
message(STATUS ${PROJECT_NAME})

# set the project name
project(${PROJECT_NAME} C)

include ("../../../Framework/Tools/CMakeLists.txt")

# For any additional source files, add them here
# There are no problems adding all source files, even if they are part of the 
# "add_executable" in the file ../../../Framework/Tools/CMakeLists.txt

# target_sources(${PROJECT_NAME} PRIVATE App_SourceFile1.c App_SourceFile2.c)
