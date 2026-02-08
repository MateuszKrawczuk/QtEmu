// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGHARDWARE_H
#define MACHINECONFIGHARDWARE_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QSpinBox>

// Local
#include "../qemu.h"

// Local
#include "../machine.h"
#include "machineconfighardwaretabs.h"

class MachineConfigHardware : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigHardware(Machine *machine,
                                     QEMU *qemuObject,
                                     QWidget *parent = nullptr);
        ~MachineConfigHardware();
        QWidget *m_hardwarePageWidget;

        // Methods
        void saveHardwareData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QTabWidget *m_hardwareTabWidget;
        QVBoxLayout *m_hardwarePageLayout;

        ProcessorConfigTab *m_processorConfigTab;
        GraphicsConfigTab *m_graphicsConfigTab;
        RamConfigTab *m_ramConfigTab;
        MachineTypeTab *m_machineTypeTab;

        Machine *m_machine;
        QEMU *m_qemuObject;

};

#endif // MACHINECONFIGHARDWARE_H
