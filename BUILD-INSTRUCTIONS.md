
# Release
Build in release without examples. You can disable examples in the main CMakeLists.txt

    option(BUILD_EXAMPLES "Build examples" OFF)

because examples rely on installed library.

Be sure you add the  "install" flag in the Build command for Release
this trigger the creation of an installation folder in the path:

    "${CMAKE_SOURCE_DIR}/../ExtLibs/QtNoid2"
    
where CMAKE_SOURCE_DIR is the position of the first CMakeList.txt of the project

This configuration is saved in the CMakeLists.txt.user file and it is not part of the 
tracked files.


