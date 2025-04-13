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

#ifndef ACCELERATORPAGE_H
#define ACCELERATORPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>

// Local
#include "../machine.h"
#include "../qemu.h"

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(Machine *machine,
                                        QEMU *qemuObject,
                                        QWidget *parent = nullptr);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_acceleratorLayout;
        Machine *m_newMachine;
        QEMU *m_qemuObject;

        QButtonGroup *m_acceleratorGroup;
        QRadioButton *m_tcgRadio = nullptr;
        
        #ifdef Q_OS_LINUX
        QRadioButton *m_kvmRadio = nullptr;
        QRadioButton *m_xenRadio = nullptr;
        #endif

        #ifdef Q_OS_WIN
        QRadioButton *m_whpxRadio = nullptr;
        QRadioButton *m_haxmRadio = nullptr;
        #endif

        #ifdef Q_OS_MACOS
        QRadioButton *m_hvfRadio = nullptr;
        #endif

    private slots:
        void updateAccelerator(QAbstractButton *button);
};


#endif // ACCELERATORPAGE_H
