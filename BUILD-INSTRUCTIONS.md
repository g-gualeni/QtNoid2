
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

## Artifact Preparation
Remove all exe files from the library
7Zip the library adding -Win or -macOS



# Publication on GitHub
- [x] Close the project, clear the build folder and reopen the project
- [x] Build the project in release
- [x] Copy the Win and macOS binary to Releases (remember to remove the executable)
- [x] Run the test (they must be successfull - or notify it in release notes)
- [x] Run the CLOC-Report.bat and update CLOC-REPORT.md
- [x] Draft the ReleaseNotes.txt doc

