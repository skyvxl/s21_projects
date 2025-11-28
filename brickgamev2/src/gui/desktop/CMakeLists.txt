cmake_minimum_required(VERSION 3.15)
project(BrickGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCES
    main.cpp
    mainWindow.cpp
    mainWindow.h
    gameController.cpp
    gameController.h
)

add_executable(BrickGame
    # MACOSX_BUNDLE
    ${SOURCES}
)

target_link_libraries(BrickGame PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/../../s21_brick_game.a
)
