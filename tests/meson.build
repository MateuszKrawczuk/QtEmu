# SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
# SPDX-License-Identifier: GPL-2.0-or-later

# Find Qt Test module and other required modules
qt6_test_dep = dependency('qt6', modules : ['Core', 'Test', 'Network', 'Widgets', 'Gui'])

# Include parent directory for source files
incdir_tests = include_directories('../src')

# Cloud-init ISO utils tests
test_cloudinitisoutils_headers = [
    '../src/utils/cloudinitisoutils.h',
    '../src/machine.h',
    '../src/networkadapter.h',
    '../src/boot.h',
    '../src/media.h',
    '../src/qemu.h',
    '../src/utils/logger.h',
    '../src/utils/systemutils.h'
]

test_cloudinitisoutils_sources = [
    'test_cloudinitisoutils.cpp',
    '../src/utils/cloudinitisoutils.cpp',
    '../src/machine.cpp',
    '../src/networkadapter.cpp',
    '../src/boot.cpp',
    '../src/media.cpp',
    '../src/qemu.cpp',
    '../src/utils/logger.cpp',
    '../src/utils/systemutils.cpp'
]

test_cloudinitisoutils_moc = qt6.preprocess(
    moc_headers: test_cloudinitisoutils_headers,
    moc_sources: ['test_cloudinitisoutils.cpp'],
    include_directories: incdir_tests
)

test_cloudinitisoutils_exe = executable(
    'test_cloudinitisoutils',
    test_cloudinitisoutils_sources,
    test_cloudinitisoutils_moc,
    dependencies: qt6_test_dep,
    include_directories: incdir_tests
)

test('test_cloudinitisoutils', test_cloudinitisoutils_exe, timeout: 30)

# Machine cloud-init properties tests
test_machine_cloudinit_headers = [
    '../src/machine.h',
    '../src/machineutils.h',
    '../src/networkadapter.h',
    '../src/boot.h',
    '../src/media.h',
    '../src/qemu.h',
    '../src/utils/logger.h',
    '../src/utils/systemutils.h',
    '../src/utils/cloudinitisoutils.h'
]

test_machine_cloudinit_sources = [
    'test_machine_cloudinit.cpp',
    '../src/machine.cpp',
    '../src/machineutils.cpp',
    '../src/networkadapter.cpp',
    '../src/boot.cpp',
    '../src/media.cpp',
    '../src/qemu.cpp',
    '../src/utils/logger.cpp',
    '../src/utils/systemutils.cpp',
    '../src/utils/cloudinitisoutils.cpp'
]

test_machine_cloudinit_moc = qt6.preprocess(
    moc_headers: test_machine_cloudinit_headers,
    moc_sources: ['test_machine_cloudinit.cpp'],
    include_directories: incdir_tests
)

test_machine_cloudinit_exe = executable(
    'test_machine_cloudinit',
    test_machine_cloudinit_sources,
    test_machine_cloudinit_moc,
    dependencies: qt6_test_dep,
    include_directories: incdir_tests
)

test('test_machine_cloudinit', test_machine_cloudinit_exe, timeout: 30)

# Machine custom arguments tests
test_machine_customargs_headers = [
    '../src/machine.h',
    '../src/machineutils.h',
    '../src/networkadapter.h',
    '../src/boot.h',
    '../src/media.h',
    '../src/qemu.h',
    '../src/utils/logger.h',
    '../src/utils/systemutils.h',
    '../src/utils/cloudinitisoutils.h'
]

test_machine_customargs_sources = [
    'test_machine_customargs.cpp',
    '../src/machine.cpp',
    '../src/machineutils.cpp',
    '../src/networkadapter.cpp',
    '../src/boot.cpp',
    '../src/media.cpp',
    '../src/qemu.cpp',
    '../src/utils/logger.cpp',
    '../src/utils/systemutils.cpp',
    '../src/utils/cloudinitisoutils.cpp'
]

test_machine_customargs_moc = qt6.preprocess(
    moc_headers: test_machine_customargs_headers,
    moc_sources: ['test_machine_customargs.cpp'],
    include_directories: incdir_tests
)

test_machine_customargs_exe = executable(
    'test_machine_customargs',
    test_machine_customargs_sources,
    test_machine_customargs_moc,
    dependencies: qt6_test_dep,
    include_directories: incdir_tests
)

test('test_machine_customargs', test_machine_customargs_exe, timeout: 30)

# NetworkAdapter tests
test_networkadapter_headers = [
    '../src/networkadapter.h'
]

test_networkadapter_sources = [
    'test_networkadapter.cpp',
    '../src/networkadapter.cpp'
]

test_networkadapter_moc = qt6.preprocess(
    moc_headers: test_networkadapter_headers,
    moc_sources: ['test_networkadapter.cpp'],
    include_directories: incdir_tests
)

test_networkadapter_exe = executable(
    'test_networkadapter',
    test_networkadapter_sources,
    test_networkadapter_moc,
    dependencies: qt6_test_dep,
    include_directories: incdir_tests
)

test('test_networkadapter', test_networkadapter_exe, timeout: 30)

# NetworkUtils tests
test_networkutils_headers = [
    '../src/networkadapter.h',
    '../src/utils/networkutils.h'
]

test_networkutils_sources = [
    'test_networkutils.cpp',
    '../src/networkadapter.cpp',
    '../src/utils/networkutils.cpp'
]

test_networkutils_moc = qt6.preprocess(
    moc_headers: test_networkutils_headers,
    moc_sources: ['test_networkutils.cpp'],
    include_directories: incdir_tests
)

test_networkutils_exe = executable(
    'test_networkutils',
    test_networkutils_sources,
    test_networkutils_moc,
    dependencies: qt6_test_dep,
    include_directories: incdir_tests
)

test('test_networkutils', test_networkutils_exe, timeout: 30)

# Summary message
message('Tests configured:')
message('  - test_cloudinitisoutils')
message('  - test_machine_cloudinit')
message('  - test_machine_customargs')
message('  - test_networkadapter')
message('  - test_networkutils')
message('Run tests with: meson test or ninja test')
