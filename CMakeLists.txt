cmake_minimum_required(VERSION 3.20)

project(QtNoid
    VERSION 2.0.0
    DESCRIPTION "${PROJECT_NAME} - Modular Qt Library version 2"
    LANGUAGES CXX
)

# =============================================================================
# Build Options
# =============================================================================
option(BUILD_CORE "Build QtNoidCore library" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)


# =============================================================================
# Installation Configuration
# =============================================================================
# Permette di sovrascrivere dalla command line
if(NOT DEFINED CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "C:/GitHub/Libs/QtNoid2" CACHE PATH "Installation directory" FORCE)
endif()


# =============================================================================
# Compiler Configuration
# =============================================================================
# Imposta lo standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# =============================================================================
# Add Subdirectories
# =============================================================================
if(BUILD_CORE)
    add_subdirectory(Core)
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
    README.md
    LICENSE
)
add_custom_target(docs SOURCES ${PROJECT_DOCS})
