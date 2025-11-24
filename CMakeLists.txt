cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(json-viewer VERSION 1.0.0 DESCRIPTION "Графический просмотрщик JSON" LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src/json-viewer)

enable_testing()
add_subdirectory(test)
