# CMakeList.txt : CMake project for SpikingNetworks::neuron, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(MODULE_NAME "neuron")
set(LIBRARY_NAME "${PROJECT_NAME}_${MODULE_NAME}")
set(LIBRARY_NAME_ALIAS "${PROJECT_NAME}::${MODULE_NAME}")


GATHER_MODULE_HEADERS()
GATHER_SOURCES()

# Add source to this project's library.
add_library(${LIBRARY_NAME} ${SOURCES})
add_library(${LIBRARY_NAME_ALIAS} ALIAS ${LIBRARY_NAME})

ADD_DEPENDENCY_MODULE(${LIBRARY_NAME} "core")
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include" )
target_sources(${LIBRARY_NAME} PUBLIC ${HEADERS})

# TODO: Add tests and install targets if needed.
