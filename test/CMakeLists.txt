cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(json-TestsJsonViewer VERSION 1.0.0 DESCRIPTION "Тесты на графический просмотрщик JSON" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src/json-viewer)
include_directories(${CMAKE_SOURCE_DIR}/src/json-viewer/include)

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(GTest REQUIRED)

add_executable(TestsJsonViewer
    ${CMAKE_SOURCE_DIR}/src/json-viewer/include/jsonmodel.h 
    ${CMAKE_SOURCE_DIR}/src/json-viewer/jsonmodel.cpp
    testjsonviewer.cpp)
target_link_libraries(TestsJsonViewer ${CMAKE_CXX_STANDARD_LIBRARIES} Qt5::Core Qt5::Widgets GTest::GTest GTest::Main) 
add_test(NAME TestsJsonViewer COMMAND TestsJsonViewer)
