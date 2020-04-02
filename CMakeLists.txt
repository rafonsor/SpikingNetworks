# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project("SpikingNetworks")

set(SN_VERSION_MAJOR 0)
set(SN_VERSION_MINOR 0)
set(SN_VERSION_PATCH 1)

set_property(GLOBAL PROPERTY CXX_STANDARD 20)
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print debug messages
set(DEBUG_LOGS 1)

# Include custom CMake functions and macros.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include("helpers")

SET_PROJECT_VERSION()

# Dependencies
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)

# Include sub-projects.
add_subdirectory("modules")
add_subdirectory("apps")
