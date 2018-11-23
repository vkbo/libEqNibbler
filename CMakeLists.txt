#
# Equation Nibbler Library Build Script
#

project(EquationNibbler)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Some Checks
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "${PROJECT_NAME} requires an out of source build.")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Defaulting to build type Release" FORCE)
endif()

# Feature Options
option(FORTRAN_INTERFACE "Build Fortran interface" OFF)
option(EXAMPLE_CPP       "Build example executable for C++" ON)
option(EXAMPLE_FORTRAN   "Build example executable for Fortran" OFF)
option(CRLIBM            "Use correctly rounded libmath instead of system libmath" OFF)
option(DEBUG             "Show debugging output" OFF)

# Build Options
option(STATIC "Create a static library." OFF)
option(32BIT  "Create a 32bit binary" OFF)
option(64BIT  "Create a 64bit binary" ON)
option(NATIVE "Enable optimisations for the current machine" OFF)

# Check Options
if(32BIT AND 64BIT)
  set(64BIT OFF)
  message(STATUS "32bit build selected, disabling 64bit option.")
endif()

# Source Files
set(EQN_SOURCES
  ${CMAKE_SOURCE_DIR}/source/libEqNibbler.hpp
  ${CMAKE_SOURCE_DIR}/source/libEqNibbler.cpp
  ${CMAKE_SOURCE_DIR}/source/clsMath.hpp
  ${CMAKE_SOURCE_DIR}/source/clsMath.cpp
)

# Configure Compiler
if(STATIC)
  set(BUILD_SHARED_LIBS OFF)
endif()
set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  set(CMAKE_CXX_FLAGS         "-Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -save-temps")
  if(32BIT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  endif(32BIT)
  if(64BIT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
  endif(64BIT)
  if(NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
  endif(NATIVE)
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Intel")
  set(CMAKE_CXX_FLAGS         "-Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
  set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
  if(32BIT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
  endif(32BIT)
  if(64BIT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
  endif(64BIT)
  if(NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
  endif(NATIVE)
else()
  message(FATAL_ERROR "Unkown compiler")
endif()

if(STATIC)
  add_library(EqNibblerLib STATIC ${EQN_SOURCES})
else()
  add_library(EqNibblerLib STATIC ${EQN_SOURCES})
endif()
set_target_properties(EqNibblerLib PROPERTIES OUTPUT_NAME "EqNibbler")
set_target_properties(EqNibblerLib PROPERTIES LINKER_LANGUAGE CXX)

if(EXAMPLE_CPP)
  add_executable(EqNEx_CPP ${CMAKE_SOURCE_DIR}/example_cpp.cpp)
  set_target_properties(EqNEx_CPP PROPERTIES OUTPUT_NAME "example_cpp.e")
  target_link_libraries(EqNEx_CPP EqNibblerLib)
endif()
