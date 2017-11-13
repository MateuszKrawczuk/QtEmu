/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
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

#ifndef MACHINEWIZARD_H
#define MACHINEWIZARD_H

// Qt
#include <QWizard>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>

// Local
#include "systemutils.h"

class MachineWizard : public QWizard {
    Q_OBJECT

    public:
        explicit MachineWizard(QWidget *parent = 0);
        ~MachineWizard();

    enum { Page_Name, Page_Memory, Page_Disk,
               Page_Hardware, Page_Conclusion };

    signals:

    public slots:

    protected:

    private:

};

class MachineNamePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNamePage(QWidget *parent = 0);
        ~MachineNamePage();

    signals:

    public slots:
        void selectOS(int OSSelected);

    protected:

    private:
        QGridLayout *mainLayout;

        QLabel *descriptionNameLabel;
        QLabel *machineNameLabel;
        QLabel *OSTypeLabel;
        QLabel *OSVersionLabel;

        QComboBox *OSType;
        QComboBox *OSVersion;

        QLineEdit *machineNameLineEdit;
};

class MachineMemoryPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineMemoryPage(QWidget *parent = 0);
        ~MachineMemoryPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *machineMemoryLayout;
        QHBoxLayout *sliderMemoryLayout;

        QSpinBox *memorySpinBox;
        QSlider *memorySlider;

        QLabel *descriptionMemoryLabel;
        QLabel *spinBoxMemoryLabel;
};

class MachineDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineDiskPage(QWidget *parent = 0);
        ~MachineDiskPage();

    signals:

    public slots:

    protected:

    private:
};

class MachineHardwarePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineHardwarePage(QWidget *parent = 0);
        ~MachineHardwarePage();

    signals:

    public slots:

    protected:

    private:
};

class MachineConclusionPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineConclusionPage(QWidget *parent = 0);
        ~MachineConclusionPage();

    signals:

    public slots:

    protected:

    private:
};

#endif // MACHINEWIZARD_H
