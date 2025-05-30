cmake_minimum_required(VERSION 3.20)

project(QtNoidCore
    VERSION 1.0.0
    DESCRIPTION "QtNoidCore - Library Life Cycle"
    LANGUAGES CXX
)

# =============================================================================
# Directory Configuration
# =============================================================================
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# =============================================================================
# Source Files
# =============================================================================
set(SOURCES
    ${SOURCES_DIR}/core.cpp
)

# File header
set(HEADERS
    ${HEADERS_DIR}/QtNoidCore/QtNoidCore  # Header convenienza
    ${HEADERS_DIR}/QtNoidCore/core.h
    ${HEADERS_DIR}/QtNoidCore/global.h
)


# =============================================================================
# Library Target
# =============================================================================
add_library(QtNoidCore SHARED
    ${SOURCES}
    ${HEADERS}
)

# Define l'export control symbol
target_compile_definitions(QtNoidCore PRIVATE
    QTNOIDCORE_LIBRARY
)

# Include directories
target_include_directories(QtNoidCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Qt Linking
target_link_libraries(QtNoidCore
    PUBLIC
        Qt6::Core
)

# Proprietà della libreria
set_target_properties(QtNoidCore PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${HEADERS}"
    OUTPUT_NAME "QtNoidCore"
)


# =============================================================================
# Installation
# =============================================================================
install(TARGETS QtNoidCore
    EXPORT QtNoidCoreTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/QtNoidCore
    COMPONENT Runtime
)

# Export dei target per standalone
install(EXPORT QtNoidCoreTargets
    FILE QtNoidCoreTargets.cmake
    NAMESPACE QtNoid::
    DESTINATION lib/cmake/QtNoidCore
    COMPONENT Development
)

# Config file per standalone
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "QtNoidCoreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/QtNoidCoreConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/QtNoidCoreConfig.cmake"
    INSTALL_DESTINATION lib/cmake/QtNoidCore
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/QtNoidCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/QtNoidCoreConfigVersion.cmake"
    DESTINATION lib/cmake/QtNoidCore
    COMPONENT Development
)


# =============================================================================
# Tests
# =============================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

