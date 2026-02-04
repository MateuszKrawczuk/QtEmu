# CLAUDE.md - QtEmu Project Guide

## Project Overview

QtEmu is a cross-platform Qt 6 GUI frontend for the QEMU emulator. It allows users to create, configure, run, export, and import virtual machines through a graphical interface. The application is written in C++17 using the Qt framework and targets Linux, Windows, macOS, and FreeBSD.

- **Version:** 2.2.0
- **License:** GNU GPL v2
- **Qt Requirement:** Qt 6.5+
- **QEMU Compatibility:** Up to QEMU 9.2

## Build Instructions

### Prerequisites

- Qt 6.5 or later (modules: Core, Gui, Widgets, Network, Svg)
- CMake 3.20+
- C++17-compatible compiler (GCC, Clang, or MSVC)
- QEMU installed on the system

### Building (CMake - primary method)

```bash
mkdir build
cd build
cmake ..
make
```

The project also supports **qmake** (`qtemu.pro`) and **Meson** (`meson.build`) as alternative build systems, but CMake is the primary and recommended method.

### Running

The built binary is `QtEmu`. On first run, a wizard prompts the user to configure the QEMU binary path and language preference. User data is stored in `~/.qtemu/`.

## Project Structure

```
QtEmu/
├── src/                         # All source code
│   ├── main.cpp                 # Entry point: QApplication setup, settings, translations, first-run wizard
│   ├── mainwindow.h/.cpp        # Main application window with machine list and details panel
│   ├── machine.h/.cpp           # VM model class: all properties, state management, QEMU process control
│   ├── qemu.h/.cpp              # QEMU binary discovery, path management, version detection
│   ├── boot.h/.cpp              # Boot configuration model (bootloader, kernel, initrd, boot order)
│   ├── media.h/.cpp             # Disk/media model (type, format, cache, I/O settings)
│   ├── machineutils.h/.cpp      # Static utilities: JSON serialization, machine file I/O, deletion
│   ├── machinewizard.h/.cpp     # New machine creation wizard (QWizard)
│   ├── configwindow.h/.cpp      # Application preferences dialog
│   ├── aboutwidget.h/.cpp       # About dialog
│   ├── helpwidget.h/.cpp        # Help/documentation viewer
│   ├── components/
│   │   └── customfilter.h/.cpp  # Filter proxy for machine type tree view
│   ├── machineconfig/           # Machine configuration tabs (opened from main window)
│   │   ├── machineconfigwindow.h/.cpp      # Configuration dialog container
│   │   ├── machineconfiggeneral.h/.cpp     # General properties tab
│   │   ├── machineconfiggeneraltabs.h/.cpp # General tab layout
│   │   ├── machineconfighardware.h/.cpp    # CPU, RAM, GPU, keyboard tab
│   │   ├── machineconfighardwaretabs.h/.cpp# Hardware tab layout
│   │   ├── machineconfigboot.h/.cpp        # Boot order tab
│   │   ├── machineconfigmedia.h/.cpp       # Disk/CD-ROM/floppy tab
│   │   ├── machineconfignetwork.h/.cpp     # Network adapter tab
│   │   ├── machineconfigaudio.h/.cpp       # Audio card tab
│   │   ├── machineconfigaccel.h/.cpp       # Acceleration tab (KVM, TCG, HAXM)
│   │   └── machineconfigbios.h/.cpp        # BIOS path configuration tab
│   ├── newmachine/              # Wizard pages for new machine creation
│   │   ├── machinepage.h/.cpp       # Machine type selection (tree + filter)
│   │   ├── generalpage.h/.cpp       # Name and OS selection
│   │   ├── hardwarepage.h/.cpp      # CPU, RAM, GPU, keyboard
│   │   ├── acceleratorpage.h/.cpp   # Acceleration method
│   │   ├── memorypage.h/.cpp        # RAM slider
│   │   ├── diskpage.h/.cpp          # Existing disk selection
│   │   └── conclusionpage.h/.cpp    # Summary confirmation
│   ├── export-import/           # VM export/import wizards
│   │   ├── export.h/.cpp                   # Export wizard
│   │   ├── exportgeneralpage.h/.cpp        # Export general settings
│   │   ├── exportdetailspage.h/.cpp        # Export metadata
│   │   ├── exportmediapage.h/.cpp          # Export disk selection
│   │   ├── import.h/.cpp                   # Import wizard
│   │   ├── importgeneralpage.h/.cpp        # Import general settings
│   │   ├── importdestinationpage.h/.cpp    # Import destination
│   │   ├── importdetailspage.h/.cpp        # Import metadata
│   │   └── importmediapage.h/.cpp          # Import media handling
│   └── utils/                   # Utility classes
│       ├── systemutils.h/.cpp       # Cross-platform system info (memory, CPU types, sound cards)
│       ├── logger.h/.cpp            # File-based logging to ~/.qtemu/logs/
│       ├── firstrunwizard.h/.cpp    # First-run configuration wizard
│       └── newdiskwizard.h/.cpp     # New disk image creation wizard
├── images/                      # Application assets
│   ├── icons/                   # Application icons
│   ├── os/                      # OS logos (32x32, 64x64) for VM list
│   └── flags/                   # Language flag icons (32x32, 64x64)
├── translations/                # Qt .ts translation files (en, pl, el, zh_CN)
├── CMakeLists.txt               # Primary build configuration
├── qtemu.pro                    # Qt qmake build configuration (legacy)
├── meson.build                  # Meson build configuration (alternative)
├── qtemu.qrc                    # Qt resource file (images + translations)
├── qtemu.rc                     # Windows resource file
└── qtemu.desktop                # Linux desktop entry
```

## Architecture

### Core Design Pattern

The application follows a **Model-View** pattern built on Qt's signal/slot mechanism:

- **Model layer:** `Machine`, `Boot`, `Media`, `QEMU` classes hold VM state and configuration data
- **View/Controller layer:** `MainWindow` and configuration dialogs present and modify data
- **Utility layer:** `MachineUtils`, `SystemUtils`, `Logger` provide static helper functions
- **Wizard pattern:** `QWizard`-based multi-page dialogs for machine creation, disk creation, export/import

### Key Classes

| Class | Role |
|-------|------|
| `MainWindow` | Main window: machine list, details panel, menus, toolbar |
| `Machine` | VM model: properties, state enum (Started/Stopped/Saved/Paused), QEMU process management |
| `QEMU` | QEMU binary management: path discovery, binary map, version detection, BIOS directory |
| `Boot` | Boot configuration: bootloader, kernel, initrd, boot order |
| `Media` | Disk/media object: type, format, cache, I/O settings |
| `MachineUtils` | Static methods for JSON serialization, machine file I/O, deletion |
| `SystemUtils` | Cross-platform system queries: total RAM, CPU types, GPU types, sound cards |
| `Logger` | Static file-based logging |

### Data Flow

1. Machine configurations are stored as JSON files in `~/.qtemu/`
2. `MachineUtils::readMachineFile()` loads JSON into `Machine` objects
3. `Machine::saveMachine()` serializes back to JSON
4. `Machine::runMachine()` builds a QEMU command line from properties and launches via `QProcess`
5. Machine state changes emit `machineStateChangedSignal` to update the UI

### Application Settings

Settings are managed via `QSettings` (platform-native storage) with these groups:
- `DataFolder` - paths to `~/.qtemu/` and logs directory
- `Configuration` - language, first-run wizard flag, QEMU binary path

## Coding Conventions

### Naming

- **Classes:** PascalCase (`MainWindow`, `MachineConfigGeneral`)
- **Methods/Functions:** camelCase (`createMenus()`, `loadMachines()`)
- **Member variables:** Mixed convention:
  - UI/process-related private members use `m_` prefix: `m_machineProcess`, `m_osListWidget`
  - Domain model members use plain camelCase: `name`, `CPUType`, `biosPath`
- **Getters:** `getXxx()` pattern (`getName()`, `getState()`)
- **Setters:** `setXxx()` pattern (`setName()`, `setState()`)
- **Signals:** descriptive with `Signal` suffix (`machineStateChangedSignal`)
- **Enums:** PascalCase values inside class scope (`Machine::States::Started`)

### File Organization

- Each class gets a `.h` / `.cpp` pair with matching lowercase filename
- Header guard format: `#ifndef CLASSNAME_H` / `#define CLASSNAME_H` / `#endif // CLASSNAME_H`
- All UI is built programmatically in C++ (no `.ui` files)

### Include Order

```cpp
// Qt
#include <QObject>
#include <QProcess>

// C++ standard library
#include <iostream>

// Local
#include "qemu.h"
#include "utils/logger.h"

// Platform-specific
#ifdef Q_OS_LINUX
#include <sys/sysinfo.h>
#endif
```

### Indentation and Formatting

- 4 spaces, no tabs
- K&R brace style (opening brace on same line)
- Constructor initializer: `ClassName::ClassName(params) : BaseClass(parent)`
- All source files include the GPL v2 license header

### Qt Patterns

- Use `connect()` with function pointers (not string-based SIGNAL/SLOT macros):
  ```cpp
  connect(m_machineProcess, &QProcess::readyReadStandardOutput,
          this, &Machine::readMachineStandardOut);
  ```
- Use `tr()` for all user-visible strings for i18n support
- Use `Q_OBJECT` macro in all QObject-derived classes
- Use `QSettings` for persistent application configuration
- Use `QProcess` for spawning QEMU instances
- Memory management relies on Qt's parent-child ownership model

### Access Specifier Order in Headers

```cpp
class MyClass : public QObject {
    Q_OBJECT

    public:
        // constructors, destructor, public methods
    signals:
        // Qt signals
    public slots:
        // public slots
    private slots:
        // private slots
    protected:
        // protected members (rarely used)
    private:
        // private data members and helper methods
};
```

## Adding New Features

### Adding a new machine configuration tab

1. Create `src/machineconfig/machineconfigXxx.h` and `.cpp`
2. Inherit from `QWidget`
3. Accept `Machine*` and `QEMU*` in the constructor
4. Build the UI programmatically in the constructor using Qt layouts
5. Register the tab in `MachineConfigWindow` constructor (add to the list widget and stacked widget)
6. Add the new files to `CMakeLists.txt` in the `qt_add_executable()` call

### Adding a new wizard page

1. Create `src/newmachine/xxxpage.h` and `.cpp`
2. Inherit from `QWizardPage`
3. Register fields with `registerField()` for cross-page data sharing
4. Add the page in `MachineWizard` constructor via `addPage()`
5. Add the new files to `CMakeLists.txt`

### Adding a new Machine property

1. Add the private member variable in `machine.h`
2. Add getter/setter methods in `machine.h` (declaration) and `machine.cpp` (implementation)
3. Update `MachineUtils::fillMachineObject()` to deserialize from JSON
4. Update `Machine::saveMachine()` to serialize to JSON
5. Add UI controls in the relevant configuration tab or wizard page

### Adding new source files

All new `.h`/`.cpp` files must be added to:
- `CMakeLists.txt` in the `qt_add_executable()` source list
- `qtemu.pro` in the `SOURCES`/`HEADERS` lists (if maintaining qmake support)
- `meson.build` source list (if maintaining Meson support)

## Platform-Specific Notes

- **Linux:** Uses `<sys/sysinfo.h>` for memory detection; KVM acceleration available
- **Windows:** Uses `<windows.h>` for memory detection; HAXM/WHPX acceleration; QEMU stdout is captured via `QTcpSocket` (port 65432) instead of pipe due to Windows limitations
- **macOS:** Uses `<sys/sysctl.h>` for memory detection; HVF acceleration
- **FreeBSD:** Uses `<sys/sysctl.h>` and `<sys/utsname.h>` for system detection

Use `#ifdef Q_OS_*` guards for platform-specific code (`Q_OS_LINUX`, `Q_OS_WIN`, `Q_OS_MAC`, `Q_OS_FREEBSD`).

## Testing

The project currently has no automated test suite. Manual testing with QEMU is required when verifying changes to command-line generation or machine lifecycle management.

## Common Pitfalls

- The `Machine` class uses `QProcess` to manage QEMU; ensure proper cleanup in destructors and slot handlers to avoid zombie processes
- JSON machine config files in `~/.qtemu/` have a specific structure; changes to serialization must maintain backward compatibility with existing saved machines
- The `QEMU` class scans for binaries by iterating the configured QEMU directory; new architectures are discovered automatically
- Translation strings must use `tr()` to remain translatable
- All images and translations are embedded via `qtemu.qrc`; new assets must be added there to be included in the build
