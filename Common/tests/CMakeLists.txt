cmake_minimum_required(VERSION 3.20)


project(QtNoidCommonTests)

# =============================================================================
# Find Required Packages
# =============================================================================
find_package(Qt6 REQUIRED COMPONENTS Test Core Gui)

# set(CMAKE_AUTOMOC ON)


function(create_qt_test test_name)
    add_executable(${test_name}
        ${test_name}.cpp
    )

    target_link_libraries(${test_name}
        Qt6::Test
        Qt6::Core
        Qt6::Gui
        QtNoidCommon
    )

    add_test(NAME ${test_name} COMMAND ${test_name})

    message(STATUS "Created test: ${test_name}")
endfunction()

# =============================================================================
# Tesing Text
# =============================================================================
create_qt_test(test_text)


# =============================================================================
# Testing Scale
# =============================================================================
create_qt_test(test_scale)


# =============================================================================
# Testing File
# =============================================================================
create_qt_test(test_file)


# =============================================================================
# Test Configuration
# =============================================================================
set_tests_properties(test_text PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen"  # Per test headless
)


