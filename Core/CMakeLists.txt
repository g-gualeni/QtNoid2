cmake_minimum_required(VERSION 3.20)



# =============================================================================
# Get parent project name and version if available
# =============================================================================
if(DEFINED CMAKE_PROJECT_NAME)
   set(PARENT_PROJECT_NAME ${CMAKE_PROJECT_NAME})
   set(PARENT_PROJECT_VERSION ${CMAKE_PROJECT_VERSION})
else()
   set(PARENT_PROJECT_NAME "QtNoid")  # Fallback name
   set(PARENT_PROJECT_VERSION "1.0.0")  # Fallback version
endif()


# =============================================================================
# Set the target project
# =============================================================================
project(QtNoidCore
    VERSION ${PARENT_PROJECT_VERSION}
    DESCRIPTION "QtNoidCore - Library Life Cycle"
    LANGUAGES CXX
)

# =============================================================================
# Directory Configuration
# =============================================================================
set(SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


# =============================================================================
# Version Header Generation
# =============================================================================
# Create uppercase project name for macros
string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)


# Configure version header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Version.h"
    @ONLY
)


# =============================================================================
# Source Files
# =============================================================================
set(SOURCES
    ${SOURCES_DIR}/core.cpp
)

# File header
set(HEADERS
    ${HEADERS_DIR}/${PROJECT_NAME}/${PROJECT_NAME}  # Header convenienza
    ${HEADERS_DIR}/${PROJECT_NAME}/core.h
    ${HEADERS_DIR}/${PROJECT_NAME}/global.h
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Version.h  # Generated version header
)


# =============================================================================
# Library Target
# =============================================================================
add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${HEADERS}
)

# Define l'export control symbol
string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${PROJECT_NAME_UPPER}_LIBRARY
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Qt Linking
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
)

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "${HEADERS}"
    OUTPUT_NAME "${PROJECT_NAME}"
)


# =============================================================================
# Installation
# =============================================================================
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
    COMPONENT Runtime
)

# Export dei target per standalone
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PARENT_PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
    COMPONENT Development
)

# Config file per standalone
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
    COMPONENT Development
)


# =============================================================================
# Tests
# =============================================================================
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

