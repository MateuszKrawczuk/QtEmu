/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef IMPORTGENERALPAGE_H
#define IMPORTGENERALPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>

// Local
#include "../machine.h"

class ImportGeneralPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportGeneralPage(QWidget *parent = nullptr);
        ~ImportGeneralPage();

    signals:

    public slots:

    private slots:
        void selectMachineConfigFile();

    protected:

    private:
        QHBoxLayout *m_machineConfigLayout;
        QVBoxLayout *m_mainLayout;

        QLineEdit *m_machineConfigLineEdit;

        QPushButton *m_machineConfigButton;
};

#endif // IMPORTGENERALPAGE_H
