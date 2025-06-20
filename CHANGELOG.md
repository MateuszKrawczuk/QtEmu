## Version 2.2.0

Features:
* Tested with QEMU up to version 9.2
* Added support for ARM64 QEMU binaries on macOS and Windows
* Added new "Others" tab for keyboard layout configuration
* Improved CPU configuration with side-by-side slider and spinbox layout
* Implemented dynamic QEMU machine type retrieval
* Added killMachine functionality to terminate QEMU process from UI
* Set default RAM to 1/4 of total available memory
* Set default CPU type to "max" and disk size to 8GB

Improvements:
* Improved disk image creation with better naming handling
* Fixed disk path handling during VM creation
* Better handling of disk name conflicts
* Simplified accelerator selection with radio buttons
* Optimized machine type selection logic
* Improved RAM slider configuration
* Better CPU core count selection logic

Bugs:
* Fixed disk creation issues when VM name contains special characters
* Fixed potential double extension issues in disk names
* Fixed building on Windows with Qt6 and MSVC
* Updated deprecated Qt API calls
* Fixed QEMU binary detection on Windows systems

## Version 2.1.1

Features:
* Ported to Qt6
* Added Cmake support
* Fixed Intel-Hda support on newer version of Qemu
* Simplified CPU configuration

Bugs:
* Minor bug fixes


## Version 2.0

QtEmu 2.0 is a complete rewrite of QtEmu.

Features:
* Ported to Qt5
* Complete redesing of the UI.
* Support for GNU/Linux, FreeBSD and Windows. Port to MacOS in progress.
* Support for versions 2.x and 3.x of QEMU
* Support to X11 and Wayland on GNU/Linux


## Version 1.0.5

Features:
* Option for custom network settings. Patch by Dawit Alemayehu <adawit at kde dot org>. Thanks!
* Possibility to set additional options which are not covered by QtEmu (e.g. "--kernel-kqemu"). Patch by Dawit Alemayehu <adawit at kde dot org>. Thanks!
* Sound configuration.
* Polish translation by Milosz Galazka <milosz at sleeplessbeastie dot eu> and Grzegorz Gibas <amigib at wp dot pl>. Thanks!
* Brazilian Portuguese translation by Jackson Miliszewski <jackwski at brturbo dot com dot br>. Thanks!
Fixes:
* Code optimization (Mosty detected by KDE Krazy check).
* Initialize all variables in machineprocess.*. Patch by Dawit Alemayehu <adawit at kde dot org>. Thanks!

## Version 1.0.4

Features:
* Manifest file added in order to see everywhere Win styles (e.g. file dialogs) (patch by Cristian Adam)
* French translation by Fathi Boudra <fboudra at free dot fr>. Thanks!
* Italian translation by Gianluca Cecchi <gianluca dot cecchi at gmail dot com>. Thanks!
* Add confirmation before closing a machine. Also inform the user that he can open the machine again.
* Add information about possible performance loose with seamless mouse mode.
* Add support for icon themes.
* Add KDE Oxygen icon theme. QtEmu fits now better in a KDE4 desktop. Please see images/oxygen/COPYING
  for license information.
Fixes:
* Allow compilation with Qt 4.1 again. Patch by Mandrake.

## Version 1.0.3

Features:
* Floppy Disk support
* Czech translation by excamo <excamo at gmail dot com>. Thanks!
* Spanish translation by Manolo Valdes <nolis71cu at gmail dot com>. Thanks!
Fixes:
* Remove Qt 4.1 support code, it does not compile there anyways.
  Add a configure check (in cmake) for Qt 4.2 (patch by Gwenole Beauchesne / Laurent Montel).
* Fix compilation on other systems than Linux and Windows (e.g. BSDs)

## Version 1.0.2

Features:
* Possibility to change the QEMU start command (e.g. for 64 bit executable "qemu-system-x86_64")
* Possibility to execute before start or after exit of QEMU shell commands (only experimental!)
* Turkish translation by Necmettin Begiter <begiter at gmail dot com> and Ugur Cetin <jnmbk at users.sourceforge dot net>. Thanks!
* Russian translation by Vasily Cheremisinov <cheremv at directimvest dot ru>. Thanks!
Fixes:
* Save state of network option
* Respect snapshot setting after start of QtEmu

## Version 1.0.1

Features:
* Support for VMWare hard disk images (*.vmdk)
* Support for QEMU *.qcow hard disk images
Fixes:
* Focus problem on machine tabs
