/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

// Qt

// Local
#include "systemutils.h"

SystemUtils::SystemUtils() {

    qDebug() << "SystemUtils created";
}

SystemUtils::~SystemUtils() {

    qDebug() << "SystemUtils destroyed";
}

void SystemUtils::getTotalMemory(int32_t &totalRAM) {

    struct sysinfo sys_info;

    if (sysinfo(&sys_info) != -1) {
        totalRAM = (((uint64_t)sys_info.totalram * sys_info.mem_unit) * 0.976562) / 1024 / 1024;
    }
}

void SystemUtils::setCPUTypesx86(QComboBox *CPUType) {

    // Intel among many others...IBM, Texas Instruments, AMD, Cyrix...
    CPUType -> addItem("486");

    // Intel
    CPUType -> addItem("Pentium");
    CPUType -> addItem("Pentium 2");
    CPUType -> addItem("Pentium 3");
    CPUType -> addItem("Intel Core Duo");
    CPUType -> addItem("Intel Core 2 Duo");
    CPUType -> addItem("Intel Atom N270");
    CPUType -> addItem("Broadwell");
    CPUType -> addItem("Broadwell no TSX");
    CPUType -> addItem("Conroe");
    CPUType -> addItem("Haswell");
    CPUType -> addItem("Haswell no TSX");
    CPUType -> addItem("Ivy Bridge");
    CPUType -> addItem("Nehalem");
    CPUType -> addItem("Penryn");
    CPUType -> addItem("Sandy Bridge");
    CPUType -> addItem("Skylake Client");
    CPUType -> addItem("Skylake Server");
    CPUType -> addItem("Westmere");

    // AMD
    CPUType -> addItem("AMD Opteron G1");
    CPUType -> addItem("AMD Opteron G2");
    CPUType -> addItem("AMD Opteron G3");
    CPUType -> addItem("AMD Opteron G4");
    CPUType -> addItem("AMD Opteron G5");
    CPUType -> addItem("AMD Phenom");
    CPUType -> addItem("AMD Athlon");

    // QEMU
    CPUType -> addItem("qemu32 Virtual CPU version 2.5+");
    CPUType -> addItem("qemu64 Virtual CPU version 2.5+");
    CPUType -> addItem("kvm32 Common 32-bit KVM processor");
    CPUType -> addItem("kvm64 Common KVM processor");

    // Host
    CPUType -> addItem("Base");
    CPUType -> addItem("Host");
    CPUType -> addItem("Max");
}

void SystemUtils::setGPUTypes(QComboBox *GPUType) {

    GPUType -> addItem("None");
    GPUType -> addItem("Default");
    GPUType -> addItem("Standard VGA(VESA 2.0)");
    GPUType -> addItem("Cirrus Logic GD5446");
    GPUType -> addItem("VMWare SVGA-II");
    GPUType -> addItem("QXL Paravirtual Graphic Card");
    GPUType -> addItem("Sun TCX Framebuffer");
    GPUType -> addItem("Sun Cgthree Framebuffer");
    GPUType -> addItem("Virtio VGA Card");
    GPUType -> addItem("Xen Framebuffer");
}

void SystemUtils::setKeyboardLayout(QComboBox *keyboardLayout) {

    keyboardLayout -> addItem("Arabic (ar)");
    keyboardLayout -> addItem("Danish (da)");
    keyboardLayout -> addItem("German (de)");
    keyboardLayout -> addItem("German (Switzerland) (de-ch)");
    keyboardLayout -> addItem("English (United Kingdom) (en-gb)");
    keyboardLayout -> addItem("English (United States) (en-us)");
    keyboardLayout -> addItem("Spanish (Spain) (es)");
    keyboardLayout -> addItem("Estonian (et)");
    keyboardLayout -> addItem("Finnish (fi)");
    keyboardLayout -> addItem("Faroese (fo)");
    keyboardLayout -> addItem("French (fr)");
    keyboardLayout -> addItem("French (Belgium) (fr-be)");
    keyboardLayout -> addItem("French (Canada) (fr-ca)");
    keyboardLayout -> addItem("French (Switzerland) (fr-ch)");
    keyboardLayout -> addItem("Croatian (hr)");
    keyboardLayout -> addItem("Hungarian (hu)");
    keyboardLayout -> addItem("Icelandic (is)");
    keyboardLayout -> addItem("Italian (it)");
    keyboardLayout -> addItem("Japanese (ja)");
    keyboardLayout -> addItem("Lithuanian (lt)");
    keyboardLayout -> addItem("Latvian (lv)");
    keyboardLayout -> addItem("Macedonian (mk)");
    keyboardLayout -> addItem("Dutch (nl)");
    keyboardLayout -> addItem("Dutch (Belgium) (nl-be)");
    keyboardLayout -> addItem("Norwegian (no)");
    keyboardLayout -> addItem("Polish (pl)");
    keyboardLayout -> addItem("Portuguese (pt)");
    keyboardLayout -> addItem("Portuguese (Brazil) (pt-br)");
    keyboardLayout -> addItem("Russian (ru)");
    keyboardLayout -> addItem("Slovenian (sl)");
    keyboardLayout -> addItem("Swedish (sv)");
    keyboardLayout -> addItem("Thai (th)");
    keyboardLayout -> addItem("Turkish (tr)");
}
