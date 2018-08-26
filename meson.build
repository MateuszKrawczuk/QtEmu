# meson.build is a fast and furious build system

project('QtEmu', 'cpp', default_options : ['cpp_std=c++14', 'warning_level=3'], version: '2.0')

qt5 = import('qt5')
qt5dep = dependency('qt5', modules : ['Core', 'Gui', 'Widgets', 'Network'])

incdir = include_directories('src')

add_global_arguments( ['-DQTONLY'] , language : 'cpp')

QtEmu_headers = [
                    'src/aboutwidget.h',
                    'src/configwindow.h',
                    'src/helpwidget.h',
                    'src/machine.h',
                    'src/machineutils.h',
                    'src/machinewizard.h',
                    'src/mainwindow.h',
                    'src/qemu.h',
                    'src/machineconfig/machineconfigaccel.h',
                    'src/machineconfig/machineconfigaudio.h',
                    'src/machineconfig/machineconfigboot.h',
                    'src/machineconfig/machineconfiggeneral.h',
                    'src/machineconfig/machineconfighardware.h',
                    'src/machineconfig/machineconfigmedia.h',
                    'src/machineconfig/machineconfignetwork.h',
                    'src/machineconfig/machineconfigwindow.h',
                    'src/newmachine/acceleratorpage.h',
                    'src/newmachine/conclusionpage.h',
                    'src/newmachine/diskpage.h',
                    'src/newmachine/generalpage.h',
                    'src/newmachine/hardwarepage.h',
                    'src/newmachine/memorypage.h',
                    'src/utils/logger.h',
                    'src/utils/newdiskwizard.h',
                    'src/utils/systemutils.h'
                ]

QtEmu_sources = [
                    'src/aboutwidget.cpp',
                    'src/configwindow.cpp',
                    'src/helpwidget.cpp',
                    'src/machine.cpp',
                    'src/machineutils.cpp',
                    'src/machinewizard.cpp',
                    'src/main.cpp',
                    'src/mainwindow.cpp',
                    'src/qemu.cpp',
                    'src/machineconfig/machineconfigaccel.cpp',
                    'src/machineconfig/machineconfigaudio.cpp',
                    'src/machineconfig/machineconfigboot.cpp',
                    'src/machineconfig/machineconfiggeneral.cpp',
                    'src/machineconfig/machineconfighardware.cpp',
                    'src/machineconfig/machineconfigmedia.cpp',
                    'src/machineconfig/machineconfignetwork.cpp',
                    'src/machineconfig/machineconfigwindow.cpp', 
                    'src/newmachine/acceleratorpage.cpp',
                    'src/newmachine/conclusionpage.cpp',
                    'src/newmachine/diskpage.cpp',
                    'src/newmachine/generalpage.cpp',
                    'src/newmachine/hardwarepage.cpp',
                    'src/newmachine/memorypage.cpp',
                    'src/utils/logger.cpp',
                    'src/utils/newdiskwizard.cpp',
                    'src/utils/systemutils.cpp'
                ]

QtEmu_resources = [
                    'qtemu.qrc'
                ]

prep = qt5.preprocess(
                    moc_headers : QtEmu_headers,
                    qresources : QtEmu_resources,
                    include_directories : incdir)

install_data('qtemu.png', install_dir : 'share/pixmaps')
install_data('qtemu.desktop', install_dir : 'share/applications')
