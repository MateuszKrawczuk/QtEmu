/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

#ifndef MACHINECONFIGWINDOW_H
#define MACHINECONFIGWINDOW_H

// Qt
#include <QWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QStackedWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QComboBox>

#include <QDebug>

// Local
#include "machine.h"

class MachineConfigWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigWindow(QUuid machineUuid,
                                     Machine::States machineState,
                                     QWidget *parent = 0);
        ~MachineConfigWindow();

    signals:

    public slots:

    private slots:
        void saveMachineSettings();
        void selectOS(int OSSelected);

    protected:

    private:
        // Main layout
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_topLayout;
        QHBoxLayout *m_buttonsLayout;

        QListWidget *m_optionsListWidget;
        QStackedWidget *m_optionsStackedWidget;

        QPushButton *m_saveButton;
        QPushButton *m_closeButton;

        QAction *m_closeAction;

        // General page
        QVBoxLayout *m_generalPageLayout;
        QFormLayout *m_machineDataLayout;
        QWidget *m_generalPageWidget;

        QGroupBox *m_machineGeneralGrpBox;

        QLineEdit *m_machineNameLineEdit;
        QLabel *m_machineUuidLabel;
        QLabel *m_machineStatusLabel;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        QPlainTextEdit *m_machineDescTextEdit;

        // Methods
        void createGeneralPage();

};

#endif // MACHINECONFIGWINDOW_H
