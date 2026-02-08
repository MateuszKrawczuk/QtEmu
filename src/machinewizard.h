// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
