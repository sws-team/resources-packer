cmake_minimum_required(VERSION 3.5)

project(ResourcesPacker LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(ENGINE_DIR ../Engine CACHE STRING "path to engine")
include(${ENGINE_DIR}/Resources/arcresources.cmake)

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    dialogresource.cpp
    resourcesmodel.cpp
    dialogresource.ui
    mainwindow.ui
    res.qrc
    ${RESOURCES_SOURCES}
    )
    

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

