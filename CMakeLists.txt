cmake_minimum_required(VERSION 3.18.2)
project(interview)


set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-std=c++20)
add_definitions(-g)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_C_FLAGS   "-g -O0")
    set(CMAKE_CXX_FLAGS "-g -O0")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_C_FLAGS   "-g -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "-g -O3 -DNDEBUG")
endif()


###################
### (1) include ###
###################
include_directories(
    01_algorithm/include 
    02_dynprog_vec/include 
    03_dynprog/include 
    04_fundalmental/include 
    05_template/include 
    10_interview/include 
    all_tests/include 
    /mnt/d/dev/cpp_dependency/boost_1_84_0 
)

##################
### (2) source ###
##################
file(GLOB SOURCES
     "01_algorithm/src/*.cpp" 
     "02_dynprog_vec/src/*.cpp" 
     "03_dynprog//src/*.cpp" 
     "04_fundalmental/src/*.cpp"
     "05_template/src/*.cpp"
     "10_interview/src/*.cpp"
     "all_tests/src/*.cpp"
)
add_executable(Test ${SOURCES})       # for building executable
# add_library(Test STATIC ${SOURCES}) # for building static lib .a
# add_library(Test SHARED ${SOURCES}) # for building shared lib .so

##################################
### (3) link path and link lib ###
##################################
target_link_libraries(Test -L/lib/x86_64-linux-gnu)
target_link_libraries(Test -lpthread)





