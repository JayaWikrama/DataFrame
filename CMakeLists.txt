cmake_minimum_required(VERSION 3.0.0)

# Project configuration
project(DataFrame LANGUAGES CXX)

# Define versioning variables
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "1")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# Include necessary modules
include(FindPkgConfig)
include(FetchContent)

# Set the default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Add an option to build tests (default: OFF)
option(BUILD_TESTS "Enable building of unit tests" OFF)

# Declare GoogleTest fetch content (only when tests are enabled)
if(BUILD_TESTS)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
  )
  FetchContent_MakeAvailable(googletest)
endif()

# Verbose compile option
option(VERBOSE "Enable verbose compile" OFF)
if(VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

option(USE_EXE_FUNC "Enable Exe Func and Its Param" ON)
if (USE_EXE_FUNC)
  add_definitions(-D__USE_EXE_FUNC)
endif()

option(USE_POST_EXE_FUNC "Enable Post Exe Func and Its Param" ON)
if (USE_POST_EXE_FUNC)
  add_definitions(-D__USE_POST_FUNC)
endif()

# Specify the source files
set(SOURCE_FILES
    src/data-frame.cpp
)

# Create static library
add_library(${PROJECT_NAME}-ar STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}-ar PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Create shared library from static library
add_library(${PROJECT_NAME}-lib SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-ar>)
set_target_properties(${PROJECT_NAME}-lib PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Create the executable
add_executable(${PROJECT_NAME} examples/main.cpp)

# Include directories for the project
set(INCLUDE_DIRS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_include_directories(${PROJECT_NAME}-ar PUBLIC ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME}-lib PUBLIC ${INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

# Link libraries to executables
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-lib)

# Add test configuration (only when tests are enabled)
if(BUILD_TESTS)
  add_executable(${PROJECT_NAME}-test test/data-frame-test.cpp)
  target_link_libraries(${PROJECT_NAME}-test PRIVATE ${PROJECT_NAME}-lib)
  target_link_libraries(${PROJECT_NAME}-test PUBLIC gtest gtest_main)
  target_include_directories(${PROJECT_NAME}-test PUBLIC ${INCLUDE_DIRS} ${GTest_INCLUDE_DIRS})
  enable_testing()
  add_test(NAME example_test COMMAND ${PROJECT_NAME}-test)
endif()

# Compiler and linker flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fPIC")

# Setup for package installation
install(TARGETS ${PROJECT_NAME}-lib
  EXPORT "${PROJECT_NAME}Targets"
  DESTINATION "/usr/lib/"
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)