# Generated from qtemu.pro.

cmake_minimum_required(VERSION 3.20)
project(QtEmu VERSION 2.2.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTORCC ON)
# Left for easy setup on new device, but it shouldn't be added to Cmake
#set(CMAKE_PREFIX_PATH "C:\\Qt\\6.4.0\\mingw_64")

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Network
        Svg
        )

qt_standard_project_setup()
qt_add_resources(SOURCES qtemu.qrc)

if(APPLE)
    set(executable_path "$<TARGET_FILE_NAME:QtEmu>.app")
else()
    set(executable_path "\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:QtEmu>")
endif()

qt_add_executable(QtEmu WIN32 MACOSX_BUNDLE
    ${SOURCES}
    src/aboutwidget.cpp src/aboutwidget.h
    src/boot.cpp src/boot.h
    src/components/customfilter.cpp src/components/customfilter.h
    src/configwindow.cpp src/configwindow.h
    src/export-import/export.cpp src/export-import/export.h
    src/export-import/exportdetailspage.cpp src/export-import/exportdetailspage.h
    src/export-import/exportgeneralpage.cpp src/export-import/exportgeneralpage.h
    src/export-import/exportmediapage.cpp src/export-import/exportmediapage.h
    src/export-import/import.cpp src/export-import/import.h
    src/export-import/importdestinationpage.cpp src/export-import/importdestinationpage.h
    src/export-import/importdetailspage.cpp src/export-import/importdetailspage.h
    src/export-import/importgeneralpage.cpp src/export-import/importgeneralpage.h
    src/export-import/importmediapage.cpp src/export-import/importmediapage.h
    src/helpwidget.cpp src/helpwidget.h
    src/machine.cpp src/machine.h
    src/machineconfig/machineconfigaccel.cpp src/machineconfig/machineconfigaccel.h
    src/machineconfig/machineconfigaudio.cpp src/machineconfig/machineconfigaudio.h
    src/machineconfig/machineconfigboot.cpp src/machineconfig/machineconfigboot.h
    src/machineconfig/machineconfiggeneral.cpp src/machineconfig/machineconfiggeneral.h
    src/machineconfig/machineconfiggeneraltabs.cpp src/machineconfig/machineconfiggeneraltabs.h
    src/machineconfig/machineconfighardware.cpp src/machineconfig/machineconfighardware.h
    src/machineconfig/machineconfighardwaretabs.cpp src/machineconfig/machineconfighardwaretabs.h
    src/machineconfig/machineconfigmedia.cpp src/machineconfig/machineconfigmedia.h
    src/machineconfig/machineconfignetwork.cpp src/machineconfig/machineconfignetwork.h
    src/machineconfig/machineconfigwindow.cpp src/machineconfig/machineconfigwindow.h
    src/machineutils.cpp src/machineutils.h
    src/machinewizard.cpp src/machinewizard.h
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.h
    src/media.cpp src/media.h
    src/newmachine/acceleratorpage.cpp src/newmachine/acceleratorpage.h
    src/newmachine/conclusionpage.cpp src/newmachine/conclusionpage.h
    src/newmachine/diskpage.cpp src/newmachine/diskpage.h
    src/newmachine/generalpage.cpp src/newmachine/generalpage.h
    src/newmachine/hardwarepage.cpp src/newmachine/hardwarepage.h
    src/newmachine/machinepage.cpp src/newmachine/machinepage.h
    src/newmachine/memorypage.cpp src/newmachine/memorypage.h
    src/qemu.cpp src/qemu.h
    src/utils/firstrunwizard.cpp src/utils/firstrunwizard.h
    src/utils/logger.cpp src/utils/logger.h
    src/utils/newdiskwizard.cpp src/utils/newdiskwizard.h
    src/utils/systemutils.cpp src/utils/systemutils.h
)

target_link_libraries(QtEmu PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Widgets
    Qt6::Svg
)
set_target_properties(QtEmu PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
        )

# App bundles on macOS have an .app suffix
if(APPLE)
    set(executable_path "$<TARGET_FILE_NAME:QtEmu>.app")
else()
    set(executable_path "\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:QtEmu>")
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR})
set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_QtEmu.cmake")

file(GENERATE OUTPUT ${deploy_script} CONTENT "
# Including the file pointed to by QT_DEPLOY_SUPPORT ensures the generated
# deployment script has access to qt_deploy_runtime_dependencies()
include(\"${QT_DEPLOY_SUPPORT}\")
qt_deploy_runtime_dependencies(
    EXECUTABLE \"${executable_path}\"
    GENERATE_QT_CONF
    VERBOSE
)")

# Omitting RUNTIME DESTINATION will install a non-bundle target to CMAKE_INSTALL_BINDIR,
# which coincides with the default value of QT_DEPLOY_BIN_DIR used above, './bin'.
# Installing macOS bundles always requires an explicit BUNDLE DESTINATION option.
install(TARGETS QtEmu     # Install to CMAKE_INSTALL_PREFIX/bin/QtEmu.exe
        BUNDLE  DESTINATION .      # Install to CMAKE_INSTALL_PREFIX/QtEmu.app/Contents/MacOS/QtEmu
        )
install(SCRIPT ${deploy_script})    # Add its runtime dependencies
