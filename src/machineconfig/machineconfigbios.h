// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINECONFIGBIOS_H
#define MACHINECONFIGBIOS_H

// Qt
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

// Local
#include "../machine.h"
#include "../qemu.h"

class MachineConfigBios : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigBios(Machine *machine,
                                   QEMU *qemuObject,
                                   QWidget *parent = nullptr);
        ~MachineConfigBios();
        QWidget *m_biosPageWidget;

        // Method
        void saveBiosData();

    signals:

    public slots:

    private slots:
        void customBiosToggled(bool checked);
        void selectBiosFile();

    protected:

    private:
        QVBoxLayout *m_biosPageLayout;
        QHBoxLayout *m_biosFileLayout;

        QCheckBox *m_useCustomBiosCheckBox;
        QLabel *m_biosFileLabel;
        QComboBox *m_biosComboBox;
        QPushButton *m_biosFilePushButton;

        Machine *m_machine;
        QEMU *m_qemuObject;
};

#endif // MACHINECONFIGBIOS_H
