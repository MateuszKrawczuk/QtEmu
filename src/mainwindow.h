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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QtNetwork>
#include <QSettings>
#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDesktopServices>
#include <QToolBar>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>

// Local
#include "machine.h"
#include "machineutils.h"
#include "machineconfig/machineconfigwindow.h"
#include "helpwidget.h"
#include "aboutwidget.h"
#include "configwindow.h"
#include "machinewizard.h"
#include "qemu.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        void createMenus();
        void createMenusActions();
        void createToolBars();

    public slots:

    private slots:
        void visitQtEmuWebsite();
        void visitQtEmuBugTracker();
        void visitQemuWebsite();
        void checkVersions();
        void quitApp();
        void createNewMachine();
        void machineOptions();
        void runMachine();
        void resetMachine();
        void pauseMachine();
        void deleteMachine();
        void loadUI(const int itemCount);
        void changeMachine(QListWidgetItem *machineItem);
        void machineStateChanged(Machine::States newState);
        void machinesMenu(const QPoint &pos);

    protected:

    private:
        // Start menus
        QMenu *m_fileMenu;
        QMenu *m_machineMenu;
        QMenu *m_helpMenu;

        QAction *m_exitAppAction;
        QAction *m_checkUpdateAppAction;
        QAction *m_preferencesAppAction;

        QAction *m_newMachineAction;
        QAction *m_addMachineAction;
        QAction *m_settingsMachineAction;
        QAction *m_removeMachineAction;
        QAction *m_groupMachineAction;

        QAction *m_helpQuickHelpAction;
        QAction *m_helpQtEmuWebsiteAction;
        QAction *m_helpQtEmuBugTrackerAction;
        QAction *m_helpQemuWebsiteAction;
        QAction *m_helpAboutAction;

        QAction *m_startMachineAction;
        QAction *m_stopMachineAction;
        QAction *m_resetMachineAction;
        QAction *m_pauseMachineAction;
        // End menus

        // Toolbar
        QToolBar *m_mainToolBar;

        // Widgets
        QWidget *m_mainWidget;
        MachineConfigWindow *m_machineConfigWindow;
        ConfigWindow *m_configWindow;
        HelpWidget *m_helpwidget;
        AboutWidget *m_aboutwidget;

        // Layouts
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_containerLayout;
        QVBoxLayout *m_groupContainerLayout;
        QFormLayout *m_machineDetailsLayout;

        QGroupBox *m_machineDetailsGroup;
        QGroupBox *m_hardwareGroup;
        QGroupBox *m_devicesGroup;
        QGroupBox *m_networkGroup;

        // List of OS
        QListWidget *m_osListWidget;
        QStackedWidget *m_osDetailsStackedWidget;
        QList<Machine *> m_machinesList;

        // Machine
        Machine *m_machine;

        // Labels
        QLabel *m_machineNameLabel;
        QLabel *m_machineOsLabel;
        QLabel *m_machineCPULabel;
        QLabel *m_machineRAMLabel;
        QLabel *m_machineGraphicsLabel;
        QLabel *m_machineAudioLabel;
        QLabel *m_machineAccelLabel;
        QLabel *m_machineDiskLabel;
        QLabel *m_machineNetworkLabel;

        // Messages
        QMessageBox *m_networkErrorMessageBox;
        QMessageBox *m_versionMessageBox;

        // QEMU
        QEMU *qemuGlobalObject;

        // Methods
        Machine* generateMachineObject(const QUuid machineUuid, const QString machineConfigPath);
        void loadMachines();
        void controlMachineActions(Machine::States state);
        void fillMachineDetailsSection(Machine *machine);
        void emptyMachineDetailsSection();

};
#endif // MAINWINDOW_H
