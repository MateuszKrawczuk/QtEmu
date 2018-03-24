/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef MEMORYPAGE_H
#define MEMORYPAGE_H

// Qt
#include <QWizard>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QGridLayout>

// Local
#include "../machine.h"
#include "../systemutils.h"

class MachineMemoryPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineMemoryPage(Machine *machine,
                                   QWidget *parent = 0);
        ~MachineMemoryPage();

    signals:

    public slots:

    protected:

    private:
        bool validatePage();

        QGridLayout *machineMemoryLayout;

        QSpinBox *memorySpinBox;
        QSlider *memorySlider;

        QLabel *descriptionMemoryLabel;
        QLabel *spinBoxMemoryLabel;
        QLabel *minMemoryLabel;
        QLabel *maxMemorylabel;

        Machine *newMachine;
};

#endif // MEMORYPAGE_H
