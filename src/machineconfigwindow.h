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
#include <QTabWidget>
#include <QTreeWidget>
#include <QCheckBox>
#include <QToolButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QDebug>

// Local
#include "machine.h"
#include "newmachine/hardwarepage.h"

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
        QTabWidget *m_generalTabWidget;
        QVBoxLayout *m_generalPageLayout;
        QWidget *m_generalPageWidget;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        // Hardware page
        QTabWidget *m_hardwareTabWidget;
        QVBoxLayout *m_hardwarePageLayout;
        QWidget *m_hardwarePageWidget;

        // Boot page
        QHBoxLayout *m_bootTreeLayout;
        QVBoxLayout *m_bootPageLayout;
        QGridLayout *m_kernelLayout;
        QWidget *m_bootPageWidget;

        QTreeWidget *m_bootTree;
        QTreeWidgetItem *m_floppyTreeItem;
        QTreeWidgetItem *m_CDROMTreeItem;
        QTreeWidgetItem *m_hardDiskTreeItem;
        QTreeWidgetItem *m_networkTreeItem;

        QCheckBox *m_bootMenuCheckBox;
        QCheckBox *m_kernelBootCheckBox;

        QToolButton *m_moveUpToolButton;
        QToolButton *m_moveDownToolButton;

        QLabel *m_kernelPathLabel;
        QLabel *m_initrdLabel;
        QLabel *m_kernelArgsLabel;

        QLineEdit *m_kernelPathLineEdit;
        QLineEdit *m_initredLineEdit;
        QLineEdit *m_kernelArgsLineEdit;

        QPushButton *m_kernelPathPushButton;
        QPushButton *m_initrdPushButton;

        // Methods
        void createGeneralPage();
        void createHardwarePage();
        void createBootPage();

};

class BasicTab: public QWidget {
    Q_OBJECT

    public:
        explicit BasicTab(Machine *machine,
                          QWidget *parent = 0);
        ~BasicTab();

    signals:

    public slots:

    private slots:
        void selectOS(int OSSelected);

    protected:

    private:
        QVBoxLayout *m_basicTabLayout;
        QFormLayout *m_basicTabFormLayout;

        QLineEdit *m_machineNameLineEdit;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        QLabel *m_machineUuidLabel;
        QLabel *m_machineStatusLabel;
};

class DescriptionTab: public QWidget {
    Q_OBJECT

    public:
        explicit DescriptionTab(Machine *machine,
                                QWidget *parent = 0);
        ~DescriptionTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_descriptionLayout;

        QLabel *m_machineDescLabel;
        QPlainTextEdit *m_machineDescTextEdit;

};

#endif // MACHINECONFIGWINDOW_H
