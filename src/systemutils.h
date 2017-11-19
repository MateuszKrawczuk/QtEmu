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

#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

// Qt
#include <QLabel>
#include <QComboBox>

#include <QDebug>

// GNU
#include <sys/sysinfo.h>

class SystemUtils {

    public:
        SystemUtils();
        ~SystemUtils();

        static void getTotalMemory(int32_t &totalRAM);
        static void setCPUTypesx86(QComboBox *CPUType);
        static void setGPUTypes(QComboBox *GPUType);
        static void setKeyboardLayout(QComboBox *keyboardLayout);

    private:

};

#endif // SYSTEMUTILS_H
