cmake_minimum_required(VERSION 3.20)

find_package(Qt6 REQUIRED COMPONENTS Core)
#
# Example Installation folder evaluation aligned with the final release folder
# like: QtNoid_2_1_0_Win_Qt_6_9_2_llvm_mingw_64_bit
#


# Evaluate the QtNoid Version
set(_qtnoid_version "${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}_${PROJECT_VERSION_PATCH}")

if(WIN32)
    set(_os_label "Win")
elseif(APPLE)
    set(_os_label "Mac")
elseif(UNIX)
    set(_os_label "Linux")
else()
    set(_os_label "${CMAKE_SYSTEM_NAME}")
endif()

string(REPLACE "." "_" _qt_version "${Qt6_VERSION}")

if(CMAKE_CXX_COMPILER MATCHES "llvm-mingw")
    set(_toolchain_label "llvm_mingw")
elseif(MINGW)
    set(_toolchain_label "mingw")
elseif(MSVC)
    set(_toolchain_label "msvc")
elseif(APPLE)
    set(_toolchain_label "clang")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(_toolchain_label "gcc")
else()
    set(_toolchain_label "${CMAKE_CXX_COMPILER_ID}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_arch_label "64_bit")
else()
    set(_arch_label "32_bit")
endif()

set(_build_folder_name "QtNoid_${_qtnoid_version}_${_os_label}_Qt_${_qt_version}_${_toolchain_label}_${_arch_label}")

set(EXAMPLES_INSTALL_PATH
    "${CMAKE_SOURCE_DIR}/Releases/${_build_folder_name}/Examples/Bin"
    CACHE PATH "Common install folder for all Examples" FORCE
)

message(STATUS "Examples install path: ${EXAMPLES_INSTALL_PATH}")

add_subdirectory(CoreBasicUsage)
add_subdirectory(CommonBasicUsage)
add_subdirectory(CommonTextBasicUsage)
add_subdirectory(AppCoreBasicUsage)
add_subdirectory(AppParameterBasicUsage)
add_subdirectory(AppParametersPageBasicUsage)
add_subdirectory(AppParametersPageBenchmark)
add_subdirectory(JsonTxt2JsonBasicUsage)
add_subdirectory(JsonYamlToJsonTestSuite)
