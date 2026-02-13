# SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
# SPDX-License-Identifier: GPL-2.0-or-later

cmake_minimum_required(VERSION 3.20)

# Enable testing
enable_testing()

# Find Qt Test module and other required modules
find_package(Qt6 REQUIRED COMPONENTS Test Core Network Widgets)

# Include parent directory for source files
include_directories(${CMAKE_SOURCE_DIR}/src)

# Helper function to create tests
function(add_qtemu_test test_name test_source)
    add_executable(${test_name} ${test_source})

    target_link_libraries(${test_name} PRIVATE
        Qt6::Core
        Qt6::Test
        Qt6::Network
        Qt6::Widgets
    )

    add_test(NAME ${test_name} COMMAND ${test_name})

    # Set test properties
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30
        LABELS "unit"
    )
endfunction()

# Cloud-init ISO utils tests
add_qtemu_test(test_cloudinitisoutils test_cloudinitisoutils.cpp)
target_sources(test_cloudinitisoutils PRIVATE
    ${CMAKE_SOURCE_DIR}/src/utils/cloudinitisoutils.cpp
    ${CMAKE_SOURCE_DIR}/src/machine.cpp
    ${CMAKE_SOURCE_DIR}/src/networkadapter.cpp
    ${CMAKE_SOURCE_DIR}/src/boot.cpp
    ${CMAKE_SOURCE_DIR}/src/media.cpp
    ${CMAKE_SOURCE_DIR}/src/qemu.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/systemutils.cpp
)

# Machine cloud-init properties tests
add_qtemu_test(test_machine_cloudinit test_machine_cloudinit.cpp)
target_sources(test_machine_cloudinit PRIVATE
    ${CMAKE_SOURCE_DIR}/src/machine.cpp
    ${CMAKE_SOURCE_DIR}/src/machineutils.cpp
    ${CMAKE_SOURCE_DIR}/src/networkadapter.cpp
    ${CMAKE_SOURCE_DIR}/src/boot.cpp
    ${CMAKE_SOURCE_DIR}/src/media.cpp
    ${CMAKE_SOURCE_DIR}/src/qemu.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/systemutils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/cloudinitisoutils.cpp
)

# Machine custom arguments tests
add_qtemu_test(test_machine_customargs test_machine_customargs.cpp)
target_sources(test_machine_customargs PRIVATE
    ${CMAKE_SOURCE_DIR}/src/machine.cpp
    ${CMAKE_SOURCE_DIR}/src/machineutils.cpp
    ${CMAKE_SOURCE_DIR}/src/networkadapter.cpp
    ${CMAKE_SOURCE_DIR}/src/boot.cpp
    ${CMAKE_SOURCE_DIR}/src/media.cpp
    ${CMAKE_SOURCE_DIR}/src/qemu.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/logger.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/systemutils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/cloudinitisoutils.cpp
)

# Summary message
message(STATUS "Tests configured:")
message(STATUS "  - test_cloudinitisoutils")
message(STATUS "  - test_machine_cloudinit")
message(STATUS "  - test_machine_customargs")
message(STATUS "Run tests with: ctest or make test")
