# QtEmu - A front-end written in Qt for the QEMU emulator
###### Copyright (C) 2022-2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
###### Copyright (C) 2017-2022 Sergio Carlavilla <carlavilla@mailbox.org>
###### Copyright (C) 2006-2009 Urs Wolfer <uwolfer@fwo.ch> and Ben Klopfenstein <benklop@gmail.com>

---
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.

   Or visit http://www.gnu.org/licenses/
---

# General runtime dependencies

* Qt 6.5 or later
* QEMU

# Dependencies for building

## Ubuntu/Debian

```bash
sudo apt-get install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools \
                     libqt6svg6-dev meson ninja-build g++ pkg-config
```

## Arch Linux

```bash
sudo pacman -S qt6-base qt6-tools qt6-svg meson ninja gcc pkgconf
```

## Fedora

```bash
sudo dnf install qt6-qtbase-devel qt6-qttools-devel qt6-qtsvg-devel \
                 meson ninja-build gcc-c++ pkgconf-pkg-config
```

## macOS

```bash
brew install qt meson ninja pkg-config
```

## Windows

1. Install Qt 6.5+ from https://www.qt.io/download
2. Install Python from https://www.python.org/downloads/
3. Install Meson and Ninja:
   ```
   pip install meson ninja
   ```
4. Install Visual Studio Build Tools or MSVC

# Build process

**Meson is the primary and recommended build system.**

## Building with Meson (recommended)

```bash
meson setup build --buildtype=release
meson compile -C build
```

## Building with CMake (alternative)

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

# Deprecation Notice

> **Warning:** CMake and qmake build support are deprecated and will be removed in the next major release (3.0.0). Please migrate to Meson for building QtEmu.

# Running the application

After building, run:

```bash
./build/qtemu
```

On Windows:

```bash
build\qtemu.exe
```

On macOS:

```bash
open build/qtemu.app
```
