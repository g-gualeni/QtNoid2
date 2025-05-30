cmake_minimum_required(VERSION 3.20)

project(QtNoid
    VERSION 1.0.0 
    DESCRIPTION "QtNoid - Modular Qt Library"
    LANGUAGES CXX
)

# =============================================================================
# Project Docs
# =============================================================================
set(PROJECT_DOCS
    README.md
)

add_custom_target(docs SOURCES ${PROJECT_DOCS})
