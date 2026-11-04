cmake_minimum_required(VERSION 3.21)
project(JsonYamlToJsonTestSuite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)


# =============================================================================
# Search for QtNoid Installed library
# =============================================================================
set(QTNOID_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../ExtLibs/QtNoid2"
    CACHE PATH "Folder where QtNoid2 is installed")

find_package(QtNoidCommon REQUIRED HINTS "${QTNOID_INSTALL_DIR}")
find_package(QtNoidJson REQUIRED HINTS "${QTNOID_INSTALL_DIR}")
find_package(QtNoidApp REQUIRED HINTS "${QTNOID_INSTALL_DIR}")
find_package(QtNoidUiCommon REQUIRED HINTS "${QTNOID_INSTALL_DIR}")


# Install folder: provided by Examples/CMakeLists.txt when built inside QtNoid,
# otherwise a local default for the standalone build
if(NOT DEFINED EXAMPLES_INSTALL_PATH)
    set(EXAMPLES_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../Releases/Standalone/Examples/Bin"
        CACHE PATH "Install folder for the example")
    message(STATUS "Examples install path: ${EXAMPLES_INSTALL_PATH}")
endif()


qt_standard_project_setup()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    recentfilesmanager.cpp
    recentfilesmanager.h
    foldercombobox.cpp
    foldercombobox.h
    frmsavedataset.cpp
    frmsavedataset.h
    frmsavedataset.ui
    frmnewdataset.cpp
    frmnewdataset.h
    frmnewdataset.ui
    frmmanageprojectresources.cpp
    frmmanageprojectresources.h
    frmmanageprojectresources.ui
    dlgfilediff.cpp
    dlgfilediff.h
    resources/JsonYamlToJsonTestSuite.qrc
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SOURCE_FILES_PATH="${CMAKE_CURRENT_SOURCE_DIR}"
)


target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
        QtNoid::QtNoidCommon
        QtNoid::QtNoidApp
        QtNoid::QtNoidJson
        QtNoid::QtNoidUiCommon
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${EXAMPLES_INSTALL_PATH}
    RUNTIME DESTINATION ${EXAMPLES_INSTALL_PATH}
    LIBRARY DESTINATION ${EXAMPLES_INSTALL_PATH}
)
install(FILES
    $<TARGET_FILE:QtNoid::QtNoidCommon>
    $<TARGET_FILE:QtNoid::QtNoidApp>
    $<TARGET_FILE:QtNoid::QtNoidJson>
    $<TARGET_FILE:QtNoid::QtNoidUiCommon>
    DESTINATION ${EXAMPLES_INSTALL_PATH}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(CODE "set(QT_DEPLOY_PREFIX \"${EXAMPLES_INSTALL_PATH}\")")
install(CODE "set(QT_DEPLOY_BIN_DIR \".\")")
install(SCRIPT ${deploy_script})


# =============================================================================
# Project Docs
# =============================================================================
set(PROJECT_DOCS
    doc/QtNoidJsonYamlToJsonTestSuite.md
)
add_custom_target(${PROJECT_NAME}Docs SOURCES ${PROJECT_DOCS})
