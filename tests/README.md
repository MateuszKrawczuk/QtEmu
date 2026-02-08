<!--
SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
SPDX-License-Identifier: GPL-2.0-or-later
-->

# QtEmu Unit Tests

This directory contains unit tests for QtEmu using the Qt Test framework.

## Building and Running Tests

### Build with tests (default)

```bash
mkdir build && cd build
cmake ..
make
```

### Run all tests

```bash
ctest
```

### Run tests with verbose output

```bash
ctest --verbose
```

### Run specific test

```bash
./test_cloudinitisoutils
./test_machine_cloudinit
```

### Run tests matching a pattern

```bash
ctest -R cloudinit
```

### Disable tests

```bash
cmake -DBUILD_TESTING=OFF ..
```

## Test Structure

### test_cloudinitisoutils.cpp
Tests for CloudInitIsoUtils utility class:
- User-data YAML generation (basic, with password, with SSH key, custom)
- Meta-data YAML generation
- Password hashing (SHA-512, security-critical)
- ISO file generation and cleanup
- YAML validation

### test_machine_cloudinit.cpp
Tests for Machine class cloud-init properties:
- Getters and setters for all cloud-init fields
- Default values initialization
- JSON serialization and deserialization
- **Security test:** Password NOT saved to JSON file

## Writing New Tests

1. Create a new test file: `test_<feature>.cpp`
2. Use Qt Test framework macros:
   ```cpp
   #include <QtTest/QtTest>

   class TestMyFeature : public QObject {
       Q_OBJECT
   private slots:
       void initTestCase();
       void cleanup();
       void testSomething();
   };

   QTEST_MAIN(TestMyFeature)
   #include "test_myfeature.moc"
   ```
3. Add the test to `tests/CMakeLists.txt`:
   ```cmake
   add_qtemu_test(test_myfeature test_myfeature.cpp)
   ```

## Test Requirements

- All new features should have unit tests
- Security-critical code **must** have tests
- Tests should be platform-independent where possible
- Use `QSKIP()` for platform-specific tests that can't run

## CI Integration

Tests run automatically in GitHub Actions:
- On every push to main branches
- On all pull requests
- Tests must pass before merge

## Dependencies

Tests require:
- Qt6 Test module
- CMake 3.20+
- C++17 compiler

For cloud-init ISO tests to fully work, optional tools:
- `mkpasswd` (for password hashing)
- `genisoimage`, `mkisofs`, or `xorriso` (for ISO creation)
- Python 3 with `passlib` (alternative password hashing)

If these tools are not available, relevant tests will be skipped with warnings.
