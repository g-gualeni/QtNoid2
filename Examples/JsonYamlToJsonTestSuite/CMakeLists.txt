cmake_minimum_required(VERSION 3.19)
project(JsonYamlToJsonTestSuite LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)


# =============================================================================
# Search for QtNoid Installed library
# =============================================================================
find_package(QtNoidCommon REQUIRED HINTS "../../../ExtLibs/QtNoid2")
find_package(QtNoidJson REQUIRED HINTS "../../../ExtLibs/QtNoid2")
find_package(QtNoidApp REQUIRED HINTS "../../../ExtLibs/QtNoid2")


qt_standard_project_setup()

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
    doc/JsonYamlToJsonTestSuite.md
)
add_custom_target(${PROJECT_NAME}Docs SOURCES ${PROJECT_DOCS})
