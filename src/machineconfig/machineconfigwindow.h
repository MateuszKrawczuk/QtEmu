// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
#include "../machine.h"
#include "../qemu.h"
#include "machineconfiggeneral.h"
#include "machineconfighardware.h"
#include "machineconfigboot.h"
#include "machineconfigmedia.h"
#include "machineconfignetwork.h"
#include "machineconfigaudio.h"
#include "machineconfigaccel.h"
#include "machineconfigbios.h"

class MachineConfigWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigWindow(Machine *machine,
                                     QEMU *QEMUGlobalObject,
                                     QListWidgetItem *osWidget,
                                     QWidget *parent = nullptr);
        ~MachineConfigWindow();

    signals:
        void saveMachineSettingsSignal(QUuid machineUuid);

    public slots:

    private slots:
        void saveMachineSettings();
        void cancelMachineSettings();

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

        // Config pages
        MachineConfigGeneral *m_configGeneral;
        MachineConfigHardware *m_configHardware;
        MachineConfigBoot *m_configBoot;
        MachineConfigMedia *m_configMedia;
        MachineConfigNetwork *m_configNetwork;
        MachineConfigAudio *m_configAudio;
        MachineConfigAccel *m_configAccel;
        MachineConfigBios *m_configBios;

        Machine *m_machine;
        QListWidgetItem *m_osWidget;

};

#endif // MACHINECONFIGWINDOW_H
