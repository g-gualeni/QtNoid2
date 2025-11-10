cmake_minimum_required(VERSION 3.16)

project(JsonYamlToJsonTestSuite VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(QtNoidCommon REQUIRED HINTS "../../../ExtLibs/QtNoid2")
find_package(QtNoidJson REQUIRED HINTS "../../../ExtLibs/QtNoid2")
find_package(QtNoidApp REQUIRED HINTS "../../../ExtLibs/QtNoid2")


add_compile_definitions(SOURCE_FILES_PATH="${CMAKE_CURRENT_SOURCE_DIR}")



set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        recentfilesmanager.cpp
        recentfilesmanager.h
        foldercombobox.cpp
        foldercombobox.h
        JsonYamlToJsonTestSuite.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(JsonYamlToJsonTestSuite
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET JsonYamlToJsonTestSuite APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(JsonYamlToJsonTestSuite SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(JsonYamlToJsonTestSuite
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(JsonYamlToJsonTestSuite
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    QtNoid::QtNoidJson
    QtNoid::QtNoidCommon
    QtNoid::QtNoidApp

)



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.JsonYamlToJsonTestSuite)
endif()
set_target_properties(JsonYamlToJsonTestSuite PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS JsonYamlToJsonTestSuite
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(JsonYamlToJsonTestSuite)
endif()
