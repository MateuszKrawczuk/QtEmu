# meson.build is a fast and furious build system

project('QtEmu', 'cpp', default_options : ['cpp_std=c++17', 'warning_level=3'], version: '2.1.1')

qt6 = import('qt6')
qt6_dep = dependency('qt6', modules : ['Core', 'Gui', 'Widgets', 'Network', 'Svg'])

incdir = include_directories('src')

add_global_arguments( ['-DQTONLY'] , language : 'cpp')

QtEmu_headers = [
                    'src/aboutwidget.h',
                    'src/boot.h',
                    'src/configwindow.h',
                    'src/helpwidget.h',
                    'src/machine.h',
                    'src/machineutils.h',
                    'src/machinewizard.h',
                    'src/mainwindow.h',
                    'src/media.h',
                    'src/qemu.h',
                    'src/components/customfilter.h',
                    'src/export-import/export.h',
                    'src/export-import/exportdetailspage.h',
                    'src/export-import/exportgeneralpage.h',
                    'src/export-import/exportmediapage.h',
                    'src/export-import/import.h',
                    'src/export-import/importdestinationpage.h',
                    'src/export-import/importdetailspage.h',
                    'src/export-import/importgeneralpage.h',
                    'src/export-import/importmediapage.h',
                    'src/machineconfig/machineconfigaccel.h',
                    'src/machineconfig/machineconfigaudio.h',
                    'src/machineconfig/machineconfigboot.h',
                    'src/machineconfig/machineconfiggeneral.h',
                    'src/machineconfig/machineconfiggeneraltabs.h',
                    'src/machineconfig/machineconfighardware.h',
                    'src/machineconfig/machineconfighardwaretabs.h',
                    'src/machineconfig/machineconfigmedia.h',
                    'src/machineconfig/machineconfignetwork.h',
                    'src/machineconfig/machineconfigwindow.h', 
                    'src/newmachine/acceleratorpage.h',
                    'src/newmachine/conclusionpage.h',
                    'src/newmachine/diskpage.h',
                    'src/newmachine/generalpage.h',
                    'src/newmachine/hardwarepage.h',
                    'src/newmachine/machinepage.h',
                    'src/newmachine/memorypage.h',
                    'src/utils/firstrunwizard.h',
                    'src/utils/logger.h',
                    'src/utils/newdiskwizard.h',
                    'src/utils/systemutils.h'
                ]

QtEmu_sources = [
                    'src/aboutwidget.cpp',
                    'src/boot.cpp',
                    'src/configwindow.cpp',
                    'src/helpwidget.cpp',
                    'src/machine.cpp',
                    'src/machineutils.cpp',
                    'src/machinewizard.cpp',
                    'src/main.cpp',
                    'src/mainwindow.cpp',
                    'src/media.cpp',
                    'src/qemu.cpp',
                    'src/components/customfilter.cpp',
                    'src/export-import/export.cpp',
                    'src/export-import/exportdetailspage.cpp',
                    'src/export-import/exportgeneralpage.cpp',
                    'src/export-import/exportmediapage.cpp',
                    'src/export-import/import.cpp',
                    'src/export-import/importdestinationpage.cpp',
                    'src/export-import/importdetailspage.cpp',
                    'src/export-import/importgeneralpage.cpp',
                    'src/export-import/importmediapage.cpp',
                    'src/machineconfig/machineconfigaccel.cpp',
                    'src/machineconfig/machineconfigaudio.cpp',
                    'src/machineconfig/machineconfigboot.cpp',
                    'src/machineconfig/machineconfiggeneral.cpp',
                    'src/machineconfig/machineconfiggeneraltabs.cpp',
                    'src/machineconfig/machineconfighardware.cpp',
                    'src/machineconfig/machineconfighardwaretabs.cpp',
                    'src/machineconfig/machineconfigmedia.cpp',
                    'src/machineconfig/machineconfignetwork.cpp',
                    'src/machineconfig/machineconfigwindow.cpp',
                    'src/newmachine/acceleratorpage.cpp',
                    'src/newmachine/conclusionpage.cpp',
                    'src/newmachine/diskpage.cpp',
                    'src/newmachine/generalpage.cpp',
                    'src/newmachine/hardwarepage.cpp',
                    'src/newmachine/machinepage.cpp',
                    'src/newmachine/memorypage.cpp',
                    'src/utils/firstrunwizard.cpp',
                    'src/utils/logger.cpp',
                    'src/utils/newdiskwizard.cpp',
                    'src/utils/systemutils.cpp'
                ]

QtEmu_resources = [
                    'qtemu.qrc'
                ]

preprocessed = qt6.preprocess(
    moc_headers: QtEmu_headers,
    qresources: QtEmu_resources,
    include_directories: incdir
)

# Dodanie ikony dla Windows
if host_machine.system() == 'windows'
    windows = import('windows')
    win_resources = windows.compile_resources('qtemu.rc')
else
    win_resources = []
endif

if host_machine.system() == 'windows' and meson.get_compiler('cpp').get_id() == 'msvc'
    add_project_link_arguments('/ENTRY:mainCRTStartup', language: 'cpp')
endif

# Utworzenie aplikacji
executable('qtemu',
           QtEmu_sources,
           preprocessed,
           win_resources,
           dependencies: qt6_dep,
           include_directories: incdir,
           install: true,
           win_subsystem: 'windows'  # Odpowiednik WIN32 z CMake
)

# Instalacja plików desktopowych dla Linuxa
if host_machine.system() == 'linux'
    install_data('qtemu.desktop', install_dir: get_option('datadir') / 'applications')
    install_data('qtemu.png', install_dir: get_option('datadir') / 'pixmaps')
endif