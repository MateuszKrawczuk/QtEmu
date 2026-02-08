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

# Summary message
message('Tests configured:')
message('  - test_cloudinitisoutils')
message('  - test_machine_cloudinit')
message('Run tests with: meson test or ninja test')
