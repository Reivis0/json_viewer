cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(JSONViewer VERSION 1.0.0 DESCRIPTION "Графический просмотрщик JSON" LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/src/json-viewer/include)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMAGE_COLLAPSE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/icons/collapse.png")
add_definitions(-DIMAGE_COLLAPSE_FILE_PATH="${IMAGE_COLLAPSE_FILE_PATH}")

set(IMAGE_EXPAND_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/icons/expand.png")
add_definitions(-DIMAGE_EXPAND_FILE_PATH="${IMAGE_EXPAND_FILE_PATH}")

set(IMAGE_OPEN_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/icons/open.png")
add_definitions(-DIMAGE_OPEN_FILE_PATH="${IMAGE_OPEN_FILE_PATH}")

set(IMAGE_UPDATE_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/icons/update.png")
add_definitions(-DIMAGE_UPDATE_FILE_PATH="${IMAGE_UPDATE_FILE_PATH}")

find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mainwindow.h
    mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jsonhighlighter.h 
    jsonhighlighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/jsonmodel.h 
    jsonmodel.cpp
    )


add_executable(JSONViewer
    ${PROJECT_SOURCES}
    )


target_link_libraries(JSONViewer Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(JSONViewer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    )
