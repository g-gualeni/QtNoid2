cmake_minimum_required(VERSION 3.20)


project(QtNoidCommonTests)

# =============================================================================
# Find Required Packages
# =============================================================================
find_package(Qt6 REQUIRED COMPONENTS Test Core)

set(CMAKE_AUTOMOC ON)

add_executable(test_text
    test_text.cpp
)

target_link_libraries(test_text
    Qt6::Test
    Qt6::Core    
    QtNoidCommon  # Direct Target to the library as artifcat of the project
)

add_test(NAME test_text COMMAND test_text)


# =============================================================================
# Proprietà dei test
# =============================================================================
set_target_properties(test_text PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# =============================================================================
# Test Configuration
# =============================================================================
set_tests_properties(test_text PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"  # Per test headless
)


