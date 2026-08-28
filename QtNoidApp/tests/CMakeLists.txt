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
        QtNoidCommon
        QtNoidApp

    )

    add_test(NAME ${test_name} COMMAND ${test_name})

    message(STATUS "Created test: ${test_name}")
endfunction()

# =============================================================================
# Tesing QtNoid::App::Core
# =============================================================================
create_qt_test(test_app_core)

# =============================================================================
# Tesing QtNoid::App::Parameter
# =============================================================================
create_qt_test(test_parameter)

# =============================================================================
# Tesing QtNoid::App::ParameterList
# =============================================================================
create_qt_test(test_parameterlist)

# =============================================================================
# Tesing QtNoid::App::ConfigFile
# =============================================================================
create_qt_test(test_config)

# =============================================================================
# Tesing QtNoid::App::ConfigFile
# =============================================================================
create_qt_test(test_configfile)

# =============================================================================
# Tesing QtNoid::App::ConfigGlobal
# =============================================================================
create_qt_test(test_configglobal)

# =============================================================================
# Tesing QtNoid::App::Development
# =============================================================================
create_qt_test(test_development)



