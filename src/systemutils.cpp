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
