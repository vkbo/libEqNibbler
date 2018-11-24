#
# Simple Math Parser Library Build Script
#

project(SimpleMathParser)
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# Some Checks
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "${PROJECT_NAME} requires an out of source build.")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Defaulting to build type Release" FORCE)
endif()

# Feature Options
option(PYTHON_INTERFACE  "Build Python interface" OFF)
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
  ${CMAKE_SOURCE_DIR}/source/libSimpleMath.hpp
  ${CMAKE_SOURCE_DIR}/source/libSimpleMath.cpp
  ${CMAKE_SOURCE_DIR}/source/clsMath.hpp
  ${CMAKE_SOURCE_DIR}/source/clsMath.cpp
)
if(PYTHON_INTERFACE)
  list(APPEND EQN_SOURCES ${CMAKE_SOURCE_DIR}/source/python_interface.cpp)
endif()

# Configure Compiler
if(STATIC)
  set(BUILD_SHARED_LIBS OFF)
endif()
set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  set(CMAKE_CXX_FLAGS         "-Wall -fPIC")
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
  add_library(SimpleMathLib STATIC ${EQN_SOURCES})
else()
  add_library(SimpleMathLib SHARED ${EQN_SOURCES})
endif()
set_target_properties(SimpleMathLib PROPERTIES OUTPUT_NAME "SimpleMath")
set_target_properties(SimpleMathLib PROPERTIES LINKER_LANGUAGE CXX)
if(DEBUG)
  target_compile_definitions(SimpleMathLib PUBLIC DEBUG=1)
endif()

if(EXAMPLE_CPP)
  add_executable(EX_CPP ${CMAKE_SOURCE_DIR}/example_cpp.cpp)
  set_target_properties(EX_CPP PROPERTIES OUTPUT_NAME "example_cpp.e")
  target_link_libraries(EX_CPP SimpleMathLib)
endif()

if(PYTHON_INTERFACE)
  add_custom_target(PYTHON_INTERFACE)
  add_custom_command(
    TARGET PYTHON_INTERFACE POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_SOURCE_DIR}/interface/simple_math.py ${CMAKE_BINARY_DIR}/simple_math.py
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_SOURCE_DIR}/interface/test.py ${CMAKE_BINARY_DIR}/test.py
  )
  add_dependencies(SimpleMathLib PYTHON_INTERFACE)
endif()

# if(PYTHON_INTERFACE)
#   set(Python_ADDITIONAL_VERSIONS 3.4)
#   find_package(PythonLibs 3 REQUIRED)
#   if(PYTHONLIBS_FOUND)
#     message(STATUS "Python3 include dirs: ${PYTHON_INCLUDE_DIRS}")
#     add_library(IF_PYTHON SHARED ${CMAKE_SOURCE_DIR}/interface/python_interface.cpp)
#     include_directories(${PYTHON_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/source)
#     target_link_libraries(IF_PYTHON ${PYTHON_LIBRARIES})
#     set_target_properties(IF_PYTHON PROPERTIES OUTPUT_NAME "simple_math")
#     set_target_properties(IF_PYTHON PROPERTIES PREFIX "")
#     set_target_properties(IF_PYTHON PROPERTIES LINKER_LANGUAGE CXX)
#     add_custom_target(IF_PYTHON_TEST)
#     add_custom_command(
#       TARGET IF_PYTHON_TEST POST_BUILD
#       COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_SOURCE_DIR}/interface/test.py ${CMAKE_BINARY_DIR}/test.py
#     )
#     add_dependencies(IF_PYTHON IF_PYTHON_TEST)
#   else()
#     message(FATAL_ERROR "Python3 dev libraries not found")
#   endif()
# endif()
