cmake_minimum_required(VERSION 3.20)

project(QtNoid
    VERSION 2.1.0
    DESCRIPTION "${PROJECT_NAME} - Modular Qt Library version 2"
    LANGUAGES CXX
)
set(LEGAL_COPYRIGHT "(C) 2025 Gianbattista Gualeni")

# =============================================================================
# Build Options
# =============================================================================
option(BUILD_CORE "Build QtNoidCore library" ON)
option(BUILD_COMMON "Build QtNoidCommon library" ON)
option(BUILD_APP "Build QtNoidApp library" ON)

option(BUILD_TESTS "Build tests" ON)

option(BUILD_EXAMPLES "Build examples" ON)


# =============================================================================
# Installation Configuration
# =============================================================================
# You can overwrite the command line
set(LIB_INSTALL_PATH "${CMAKE_SOURCE_DIR}/../ExtLibs/QtNoid2")
if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${LIB_INSTALL_PATH}" CACHE PATH "Installation directory" FORCE)
endif()


# =============================================================================
# Compiler Configuration
# =============================================================================
# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# =============================================================================
# Add Subdirectories
# =============================================================================
if(BUILD_CORE)
    add_subdirectory(QtNoidCore)
endif()

if(BUILD_COMMON)
    add_subdirectory(QtNoidCommon)
endif()

if(BUILD_APP)
    add_subdirectory(QtNoidApp)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif()


# =============================================================================
# Package Configuration
# =============================================================================
include(CMakePackageConfigHelpers)


# =============================================================================
# Project Docs
# =============================================================================
set(PROJECT_DOCS
    ReleaseNotes.txt
    README.md
    DEVELOPMENT.md
    BUILD-INSTRUCTIONS.md
    CLOC-REPORT.md
    CLOC-Report.bat
    LICENSE
    UserStories/QtNoid.odp
    UserStories/QtNoidCore_UserStories.odp
    UserStories/QtNoidCommon_UserStories.odp
    UserStories/QtNoidApp_UserStories.odp
)
add_custom_target(${PROJECT_NAME}Docs SOURCES ${PROJECT_DOCS})

