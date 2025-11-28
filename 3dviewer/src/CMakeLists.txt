cmake_minimum_required(VERSION 3.15)
project(CPP4_3D_VIEWER VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COMPILE_FLAGS)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Test)
find_package(GTest REQUIRED)

add_compile_options(-Wall -Wextra -Werror -g -O3)

set(SOURCES
    main.cpp
    views/mainWindow.cpp
    views/glWidget.cpp
    facade/viewerFacade.cpp
    resources/shaders.qrc
    models/model.cpp
    controllers/controller.cpp
    utils/logger.cpp
)

set(HEADERS
    views/mainWindow.h
    views/glWidget.h
    facade/viewerFacade.h
    models/model.h
    controllers/controller.h
    utils/logger.h
)

if(APPLE)
    add_executable(CPP4_3D_VIEWER MACOSX_BUNDLE ${SOURCES} ${HEADERS})

    set_target_properties(CPP4_3D_VIEWER PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.s21_sandersm_spinnetc.cpp4_3d_viewer
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/resources/Info.plist.in
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE "Icon.icns"
    )

    set_source_files_properties(
        ${CMAKE_SOURCE_DIR}/resources/Icon.icns
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    target_sources(CPP4_3D_VIEWER PRIVATE
        ${CMAKE_SOURCE_DIR}/resources/Icon.icns
    )
else()
    add_executable(CPP4_3D_VIEWER ${SOURCES} ${HEADERS})
endif()

target_link_libraries(CPP4_3D_VIEWER PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets)

# TEST
set(TEST_SOURCES
    tests/3DViewerTests.cpp
    models/model.cpp
    controllers/controller.cpp
    views/glWidget.cpp
    facade/viewerFacade.cpp
    utils/logger.cpp
)

set(TEST_HEADERS
    models/model.h
    controllers/controller.h
    views/glWidget.h
    facade/viewerFacade.h
    utils/logger.h
)

add_executable(Tests ${TEST_SOURCES} ${TEST_HEADERS})

target_include_directories(Tests PRIVATE
    controllers/
    models/
)

target_link_libraries(Tests
    GTest::gtest
    GTest::gtest_main
    Qt6::Core
    Qt6::Test
    Qt6::OpenGLWidgets
)

target_compile_options(Tests PRIVATE -O0 --coverage)
target_link_options(Tests PRIVATE --coverage)

find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)

if(GCOV_PATH AND LCOV_PATH AND GENHTML_PATH)
    set(COVERAGE_DIR ${CMAKE_BINARY_DIR}/coverage)

    add_custom_target(gcov_report
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${COVERAGE_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
        COMMAND ${LCOV_PATH} --gcov-tool ${GCOV_PATH} --directory ${CMAKE_BINARY_DIR} --zerocounters
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${LCOV_PATH} --rc derive_function_end_line=0 --gcov-tool ${GCOV_PATH} --directory ${CMAKE_BINARY_DIR} --base-directory ${CMAKE_SOURCE_DIR} --capture --output-file ${COVERAGE_DIR}/coverage.info --no-external --ignore-errors inconsistent,source,format,empty,unused,mismatch
        COMMAND ${LCOV_PATH} --rc derive_function_end_line=0 --extract ${COVERAGE_DIR}/coverage.info "${CMAKE_CURRENT_SOURCE_DIR}/models/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/models/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/controllers/*.h" --ignore-errors inconsistent,source,format,empty,unused,mismatch --output-file ${COVERAGE_DIR}/coverage.info
        COMMAND ${LCOV_PATH} --rc derive_function_end_line=0 --remove ${COVERAGE_DIR}/coverage.info '*/Tests_autogen/*' "${CMAKE_CURRENT_SOURCE_DIR}/tests/*" "${CMAKE_CURRENT_SOURCE_DIR}/views/*" --ignore-errors inconsistent,source,format,empty,unused,mismatch --output-file ${COVERAGE_DIR}/coverage.info
        COMMAND ${GENHTML_PATH} --rc derive_function_end_line=0 ${COVERAGE_DIR}/coverage.info --output-directory ${COVERAGE_DIR}/html --ignore-errors inconsistent,source,format,empty,unused,category
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage report in ${COVERAGE_DIR}/html"
        VERBATIM
        DEPENDS Tests
    )
else()
    message(WARNING "gcov_report target requires gcov, lcov, and genhtml to be available")
endif()

# Enable CTest
enable_testing()

# Регистрация тестов для CTest
include(GoogleTest)
gtest_discover_tests(Tests)

add_custom_target(run
    COMMENT "run"
    DEPENDS CPP4_3D_VIEWER
    COMMAND ./CPP4_3D_VIEWER
)

add_custom_target(install_
    COMMENT "install"
    COMMAND sudo apt install Qt6-base-dev Qt6-tools-dev
)

add_custom_target(uninstall
    COMMENT "uninstall"
    COMMAND sudo apt remove -y --purge Qt6-default qtbase5-dev Qt6-qmake
)

add_custom_target(dist
    COMMENT "Creating distribution archive 3dviewer-v2.0.tar.gz"
    COMMAND cd ../.. && tar -czf 3dviewer-v2.0.tar.gz src
)

add_custom_target(dvi
    COMMENT "dvi"
    COMMAND doxygen ../docs/Doxyfile
)