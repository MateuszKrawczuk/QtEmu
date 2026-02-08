// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGACCEL_H
#define MACHINECONFIGACCEL_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QTreeWidget>
#include <QButtonGroup>
#include <QRadioButton>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"

class MachineConfigAccel : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigAccel(Machine *machine,
                                    QWidget *parent = nullptr);
        ~MachineConfigAccel();

        // Methods
        void saveAccelData();

    // Make this public so it can be accessed from outside
    QWidget *m_acceleratorPageWidget;

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_accelTreeLayout;
        QVBoxLayout *m_acceleratorLayout;

        QButtonGroup *m_acceleratorGroup;
        QRadioButton *m_tcgRadio;
        
        #ifdef Q_OS_LINUX
        QRadioButton *m_kvmRadio;
        QRadioButton *m_xenRadio;
        #endif

        #ifdef Q_OS_WIN
        QRadioButton *m_whpxRadio;
        QRadioButton *m_haxmRadio;
        #endif

        #ifdef Q_OS_MACOS
        QRadioButton *m_hvfRadio;
        #endif

        QToolButton *m_moveUpAccelToolButton;
        QToolButton *m_moveDownAccelToolButton;
        QTreeWidget *m_acceleratorTree;
        QTreeWidgetItem *m_treeItem;
        Machine *m_machine;

        // Methods
        void moveUpButton();
        void moveDownButton();
};

#endif // MACHINECONFIGACCEL_H
