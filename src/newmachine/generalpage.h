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

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

// Qt
#include <QWizardPage>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QGridLayout>
#include <QSettings>
#include <QDir>
#include <QLineEdit>

// Local
#include "../machine.h"
#include "../systemutils.h"

class MachineNamePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNamePage(Machine *machine,
                                 QWidget *parent = 0);
        ~MachineNamePage();

    signals:

    public slots:
        void selectOS(int OSSelected);

    protected:

    private:
        void initializePage();
        bool validatePage();
        void cleanupPage();

        QString machineFolderCreated;

        QGridLayout *mainLayout;

        QLabel *descriptionNameLabel;
        QLabel *machineNameLabel;
        QLabel *OSTypeLabel;
        QLabel *OSVersionLabel;

        QComboBox *OSType;
        QComboBox *OSVersion;

        QLineEdit *machineNameLineEdit;

        QMessageBox *createMachineMessageBox;

        Machine *newMachine;
};

#endif // GENERALPAGE_H
