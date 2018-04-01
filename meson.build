# meson.build is a fast and furious build system

project('QtEmu', 'cpp', default_options : ['cpp_std=c++14', 'warning_level=3'])

qt5 = import('qt5')
qt5dep = dependency('qt5', modules : ['Core', 'Gui', 'Widgets', 'Network'])

incdir = include_directories('src')

add_global_arguments( ['-DQTONLY'] , language : 'cpp')

QtEmu_headers =   [
                   'src/aboutwidget.h',
                   'src/configwindow.h',
                   'src/helpwidget.h',
                   'src/machine.h',
                   'src/machinewizard.h',
                   'src/mainwindow.h',
                   'src/utils/logger.h',
                   'src/utils/systemutils.h',
                   'src/newmachine/acceleratorpage.h',
                   'src/newmachine/conclusionpage.h',
                   'src/newmachine/diskpage.h',
                   'src/newmachine/generalpage.h',
                   'src/newmachine/hardwarepage.h',
                   'src/newmachine/memorypage.h'
	              ]

QtEmu_sources =   [
                   'src/aboutwidget.cpp',
				   'src/configwindow.cpp',
				   'src/helpwidget.cpp',
                   'src/machine.cpp',
                   'src/machinewizard.cpp',
                   'src/main.cpp',
                   'src/mainwindow.cpp',
                   'src/utils/logger.cpp',
                   'src/utils/systemutils.cpp',
                   'src/newmachine/acceleratorpage.cpp',
                   'src/newmachine/conclusionpage.cpp',
                   'src/newmachine/diskpage.cpp',
                   'src/newmachine/generalpage.cpp',
                   'src/newmachine/hardwarepage.cpp',
                   'src/newmachine/memorypage.cpp'
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
