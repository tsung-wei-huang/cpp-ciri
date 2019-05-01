# CMake version
cmake_minimum_required (VERSION 3.9)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(Cpp-Ciri VERSION 0.0.1 LANGUAGES CXX)

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# Compiler vendors
## g++
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.3")
    message(FATAL_ERROR "\nCpp-Ciri requires g++ at least v7.3")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
## clang++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0")
    message(FATAL_ERROR "\nCpp-Ciri requires clang++ at least v6.0")
  endif() 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
## microsoft visual c++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if(NOT MSVC_VERSION GREATER_EQUAL 1914)
    message(FATAL_ERROR "\nCpp-Ciri requires MSVC++ at least v14.14") 
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /W3")
  add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE)
else()
  message(FATAL_ERROR "\n\
Cpp-Ciri currently supports the following compilers:\n\
- g++ v7.3 or above\n\
- clang++ v6.0 or above\n\
- MSVC++ v19.14 or above\n\
")
endif()

# CXX target properties
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# build options
option(CIRI_BUILD_EXAMPLES "Enables build of examples" ON)
option(CIRI_BUILD_TESTS "Enables build of tests" ON)
option(CIRI_BUILD_BENCHMARKS "Enables build of benchmarks" OFF)

# installation path
set(CIRI_INC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CIRI_LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(CIRI_UTEST_DIR ${PROJECT_SOURCE_DIR}/unittest)
set(CIRI_EXAMPLE_DIR ${PROJECT_SOURCE_DIR}/example)
set(CIRI_BENCHMARK_DIR ${PROJECT_SOURCE_DIR}/benchmark)

message(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER_VERSION: " ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " ${CMAKE_EXE_LINKER_FLAGS})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_PREFIX_PATH: " ${CMAKE_PREFIX_PATH})
message(STATUS "PROJECT_NAME: " ${PROJECT_NAME})
message(STATUS "CIRI_BUILD_EXAMPLES: " ${CIRI_BUILD_EXAMPLES})
message(STATUS "CIRI_BUILD_TESTS: " ${CIRI_BUILD_TESTS})
message(STATUS "CIRI_BUILD_BENCHMARKS: " ${CIRI_BUILD_BENCHMARKS})
message(STATUS "CIRI_INC_INSTALL_DIR: " ${CIRI_INC_INSTALL_DIR})
message(STATUS "CIRI_LIB_INSTALL_DIR: " ${CIRI_LIB_INSTALL_DIR})
message(STATUS "CIRI_UTEST_DIR: " ${CIRI_UTEST_DIR})
message(STATUS "CIRI_EXAMPLE_DIR: " ${CIRI_EXAMPLE_DIR})
message(STATUS "CIRI_BENCHMARK_DIR: " ${CIRI_BENCHMARK_DIR})

# add the binary tree to the search path for include files
include_directories(${PROJECT_SOURCE_DIR})

# -----------------------------------------------------------------------------
# must-have package include
# -----------------------------------------------------------------------------

# Enable test
include(CTest)

# -----------------------------------------------------------------------------
# Cpp-Ciri library interface
# -----------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/> 
)

# -----------------------------------------------------------------------------
# Example program 
# -----------------------------------------------------------------------------

if(${CIRI_BUILD_EXAMPLES})

message(STATUS "Building examples ...")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CIRI_EXAMPLE_DIR})

add_executable(simple ${CIRI_EXAMPLE_DIR}/simple.cpp)
target_link_libraries(simple ${PROJECT_NAME})

endif()

# -----------------------------------------------------------------------------
# Unittest
# -----------------------------------------------------------------------------

if(${CIRI_BUILD_TESTS})
 
enable_testing()
message(STATUS "Building unit tests ...")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CIRI_UTEST_DIR})

# unittest for taskflow
add_executable(ciri_test unittest/ciri_test.cpp)
target_link_libraries(ciri_test ${PROJECT_NAME})
target_include_directories(ciri_test PRIVATE ${PROJECT_SOURCE_DIR}/doctest)
add_test(pod           ${CIRI_UTEST_DIR}/ciri_test -tc=POD)
add_test(pod-struct    ${CIRI_UTEST_DIR}/ciri_test -tc=POD-Struct)
add_test(string        ${CIRI_UTEST_DIR}/ciri_test -tc=string)
add_test(wstring       ${CIRI_UTEST_DIR}/ciri_test -tc=wstring)
add_test(u16string     ${CIRI_UTEST_DIR}/ciri_test -tc=u16string)
add_test(u32string     ${CIRI_UTEST_DIR}/ciri_test -tc=u32string)
add_test(vector        ${CIRI_UTEST_DIR}/ciri_test -tc=vector)
add_test(deque         ${CIRI_UTEST_DIR}/ciri_test -tc=deque)
add_test(list          ${CIRI_UTEST_DIR}/ciri_test -tc=list)
add_test(forward_list  ${CIRI_UTEST_DIR}/ciri_test -tc=forward_list)
add_test(map           ${CIRI_UTEST_DIR}/ciri_test -tc=map)
add_test(unordered_map ${CIRI_UTEST_DIR}/ciri_test -tc=unordered_map)
add_test(set           ${CIRI_UTEST_DIR}/ciri_test -tc=set)
add_test(unordered_set ${CIRI_UTEST_DIR}/ciri_test -tc=unordered_set)
add_test(array         ${CIRI_UTEST_DIR}/ciri_test -tc=array)
add_test(time_point    ${CIRI_UTEST_DIR}/ciri_test -tc=time_point)
add_test(optional      ${CIRI_UTEST_DIR}/ciri_test -tc=optional)

endif()



