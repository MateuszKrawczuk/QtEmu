/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef MACHINEWIZARD_H
#define MACHINEWIZARD_H

// Qt
#include <QWizard>
#include <QListWidget>
#include <QFile>

#include <QDebug>

// Local
#include "machine.h"
#include "qemu.h"
#include "utils/systemutils.h"

#include "newmachine/generalpage.h"
#include "newmachine/machinepage.h"
#include "newmachine/hardwarepage.h"
#include "newmachine/acceleratorpage.h"
#include "newmachine/memorypage.h"
#include "newmachine/diskpage.h"
#include "newmachine/conclusionpage.h"

class MachineWizard : public QWizard {
    Q_OBJECT

    public:
        explicit MachineWizard(Machine *machine,
                               QListWidget *osListWidget,
                               QEMU *QEMUGlobalObject,
                               QWidget *parent = nullptr);
        ~MachineWizard();

        enum { Page_Name, Page_Machine, Page_Hardware, Page_Accelerator,
               Page_Memory, Page_Disk, Page_New_Disk, Page_Conclusion };

    signals:

    public slots:

    protected:

    private:

};

#endif // MACHINEWIZARD_H
