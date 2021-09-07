cmake_minimum_required(VERSION 3.5)
project(machinegui VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_VERSION_STRING "${CMAKE_PROJECT_VERSION}")

option(GENERATE_BUNDLE "Build a bundle/installer for the current plateform" OFF)

configure_file(version_config.h.in ${CMAKE_BINARY_DIR}/generated/version_config.h)
include_directories( ${CMAKE_BINARY_DIR}/generated/ ) # Make sure it can be included...

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 5.15 COMPONENTS Widgets SerialPort REQUIRED)

add_executable(machinegui WIN32)

target_sources(machinegui PRIVATE
    addtraydialog.cpp
    addtraydialog.h
    calibrationdialog.cpp
    calibrationdialog.h
    calibrationvalues.cpp
    calibrationvalues.h
    deviceManager.cpp
    deviceManager.h
    imagedialog.cpp
    imagedialog.h
    main.cpp
    mainwindow.cpp
    imagedialog.h
    traysequenceitem.cpp
    traysequenceitem.h

    addtraydialog.ui
    calibrationdialog.ui
    imagedialog.ui
    mainwindow.ui
)

target_link_libraries(machinegui Qt5::Widgets Qt5::SerialPort)
install(TARGETS machinegui COMPONENT base)

# We set a frew variables that are common to any cpack generator, see https://cmake.org/cmake/help/latest/module/CPack.html#variables-common-to-all-cpack-generators
set(CPACK_PACKAGE_NAME "Machine Interface")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A gui program to control the Level 3 Dispenser")
set(CPACK_PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MachineInterface")
set(CPACK_PACKAGE_EXECUTABLES "machinegui;MachineGUI")
set(CPACK_IFW_PACKAGE_TITLE "Installer for MachineGUI")
set(CPACK_PACKAGE_FILE_NAME "installer")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Installation Tool")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_COMPONENTS_ALL base)

# Keeps stuff clean...
add_custom_command(
    TARGET machinegui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/libs
)

if(GENERATE_BUNDLE)
    include(cmake/DeployQt.cmake)
    # At this point, we grab the dependencies of the program. This is handled by different "deployers"
    if(WIN32)
        list(APPEND CPACK_GENERATOR NSIS)
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_NSIS_DISPLAY_NAME ${CPACK_PACKAGE_NAME})
        set(CMAKE_INSTALL_BINDIR bin)
        windeployqt(machinegui)
    elseif(APPLE)
        macdeployqt(booktracter)
    elseif(UNIX)
        list(APPEND CPACK_GENERATOR External)
        include(GNUInstallDirs) #Sometimes, the correct paths dont get set for some reason...
        linuxdeployqt(machinegui)
    endif()

    # Sets a task to place the shared libraries in the build directory "bundle" folder and place it in the install binary directory (where the binary is).
    # Furthermore, the COMPONENT requirement makes it such that this only happens when the "base" conponent is activitated (which occurs during install)
    install(
        DIRECTORY ${CMAKE_BINARY_DIR}/libs/
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT base
    )

    include(CPack REQUIRED)
    include(CPackComponent REQUIRED)

    # The base component. This is where it is defined...
    cpack_add_component(
        base
        DISPLAY_NAME "Base Program"
        DESCRIPTION "Install me"
        REQUIRED
    )
endif()
