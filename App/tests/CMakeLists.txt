cmake_minimum_required(VERSION 3.20)


project(QtNoidAppTests)

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
        QtNoidApp
    )

    add_test(NAME ${test_name} COMMAND ${test_name})

    message(STATUS "Created test: ${test_name}")
endfunction()

# =============================================================================
# Tesing first xxxxx
# =============================================================================
create_qt_test(test_settings)


# =============================================================================
# Test Configuration
# =============================================================================


