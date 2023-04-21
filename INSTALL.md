
 # QtEmu - A front-end written in Qt for the QEMU emulator
 ###### Copyright (C) 2022-2023 Mateusz Krawczuk <mat krawczuk gmail com>
 ###### Copyright (C) 2017-2022 Sergio Carlavilla <carlavilla  mailbox org>
 ###### Copyright (C) 2006-2009 Urs Wolfer <uwolfer at fwo ch> and Ben Klopfenstein <benklop gmail com>

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

* Qt 6.3 or later
* QEMU

# Dependencies for building

  - Archlinux and Parabola (runtime deps include build-time deps):
    **qt6-base**

    **Build tools: gcc, make**

# Build process

From QtEmu's main directory, where qtemu.pro is located, execute:

        mkdir build               # Create a clean directory for the build
        cd build                  # Go into it
        cmake ..                  # Ask Qmake to generate a Makefile[*]
        make                      # Run Make to compile the project

[*]you might need to use the command 'cmake' instead
