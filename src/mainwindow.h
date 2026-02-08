// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

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
#include "export-import/export.h"
#include "export-import/import.h"

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
        void exportMachine();
        void importMachine();
        void runMachine();
        void resetMachine();
        void pauseMachine();
        void deleteMachine();
        void loadUI(const int machineCount);
        void changeMachine(QListWidgetItem *machineItem);
        void machineStateChanged(Machine::States newState);
        void machinesMenu(const QPoint &pos);
        void updateMachineDetailsConfig(const QUuid machineUuid);
        void killMachine();

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
        QAction *m_exportMachineAction;
        QAction *m_importMachineAction;
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
        QAction *m_killMachineAction;
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
        QLabel *m_machineNetworkLabel;
        QLabel *m_machineMediaLabel;

        // QEMU
        QEMU *qemuGlobalObject;

        // Methods
        void generateMachineObject(const QJsonObject machinesConfigJsonObject, int pos);
        void loadMachines();
        void controlMachineActions(Machine::States state);
        void fillMachineDetailsSection(Machine *machine);
        void emptyMachineDetailsSection();

};
#endif // MAINWINDOW_H
