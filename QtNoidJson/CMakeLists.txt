cmake_minimum_required(VERSION 3.20)



# =============================================================================
# Get parent project name and version if available
# =============================================================================
if(DEFINED CMAKE_PROJECT_NAME)
   set(PARENT_PROJECT_NAME ${CMAKE_PROJECT_NAME})
   set(PARENT_PROJECT_VERSION ${CMAKE_PROJECT_VERSION})
else()
   set(PARENT_PROJECT_NAME "QtNoid")  # Fallback name
   set(PARENT_PROJECT_VERSION "2.2.0")  # Fallback version
endif()


# =============================================================================
# Set the target project
# =============================================================================
project(QtNoidJson
    VERSION ${PARENT_PROJECT_VERSION}
    DESCRIPTION "QtNoidJson - JSON Utilities"
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
    ${SOURCES_DIR}/yaml2json.cpp
)

# File header
set(HEADERS
    ${HEADERS_DIR}/${PROJECT_NAME}/${PROJECT_NAME}
    ${HEADERS_DIR}/${PROJECT_NAME}/global.h
    ${HEADERS_DIR}/${PROJECT_NAME}/yaml2json.h
)


# =============================================================================
# Third party: rapidyaml (https://github.com/biojppm/rapidyaml), MIT license
# Built as a static library and linked PRIVATE: it is not exposed to the users
# of QtNoidJson.
# =============================================================================
include(FetchContent)
set(RYML_DEFAULT_CALLBACK_USES_EXCEPTIONS ON CACHE BOOL "ryml exception classes" FORCE)
set(RYML_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(ryml
    GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
    GIT_TAG        v0.16.0
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  _download_only_   # download only: the project is added below
)
FetchContent_MakeAvailable(ryml)
# EXCLUDE_FROM_ALL: ryml is only used to build QtNoidJson, and its files are
# not copied by "cmake --install"
add_subdirectory(${ryml_SOURCE_DIR} ${ryml_BINARY_DIR} EXCLUDE_FROM_ALL)
set_target_properties(ryml PROPERTIES POSITION_INDEPENDENT_CODE ON)


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
        QtNoidCommon
    PRIVATE
        $<BUILD_INTERFACE:ryml::ryml>
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




# =============================================================================
# Project Docs
# =============================================================================
set(PROJECT_DOCS
    doc/QtNoidJson.md
    doc/QtNoidJsonYaml2Json.md
)
add_custom_target(${PROJECT_NAME}Docs SOURCES ${PROJECT_DOCS})
