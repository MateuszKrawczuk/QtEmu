// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-FileCopyrightText: 2019 sharpbai <sharpbai@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef ACCELERATORPAGE_H
#define ACCELERATORPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>

// Local
#include "../machine.h"
#include "../qemu.h"

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(Machine *machine,
                                        QEMU *qemuObject,
                                        QWidget *parent = nullptr);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_acceleratorLayout;
        Machine *m_newMachine;
        QEMU *m_qemuObject;

        QButtonGroup *m_acceleratorGroup;
        QRadioButton *m_tcgRadio = nullptr;
        
        #ifdef Q_OS_LINUX
        QRadioButton *m_kvmRadio = nullptr;
        QRadioButton *m_xenRadio = nullptr;
        #endif

        #ifdef Q_OS_WIN
        QRadioButton *m_whpxRadio = nullptr;
        QRadioButton *m_haxmRadio = nullptr;
        #endif

        #ifdef Q_OS_MACOS
        QRadioButton *m_hvfRadio = nullptr;
        #endif

    private slots:
        void updateAccelerator(QAbstractButton *button);
};


#endif // ACCELERATORPAGE_H
