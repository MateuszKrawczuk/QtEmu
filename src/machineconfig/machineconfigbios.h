/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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
