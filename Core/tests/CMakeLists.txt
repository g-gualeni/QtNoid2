cmake_minimum_required(VERSION 3.20)

project(QtNoidCommonTests)

# =============================================================================
# Find Required Packages
# =============================================================================
find_package(Qt6 REQUIRED COMPONENTS Test Core)

set(CMAKE_AUTOMOC ON)

add_executable(test_core
    test_core.cpp
)

target_link_libraries(test_core
    Qt6::Test
    Qt6::Core
    QtNoidCore  # Target diretto dalla libreria principale
)

add_test(NAME test_core COMMAND test_core)


# =============================================================================
# Proprietà dei test
# =============================================================================
set_target_properties(test_core PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# =============================================================================
# Test Configuration
# =============================================================================
set_tests_properties(test_core PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"  # Per test headless
)


