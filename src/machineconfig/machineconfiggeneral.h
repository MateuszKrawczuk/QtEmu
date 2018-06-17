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
#ifndef MACHINECONFIGGENERAL_H
#define MACHINECONFIGGENERAL_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QLineEdit>

// Local
#include "../machine.h"

class MachineConfigGeneral : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigGeneral(Machine *machine,
                                      QWidget *parent = 0);
        ~MachineConfigGeneral();

        QWidget *m_generalPageWidget;

    signals:

    public slots:

    private slots:

    protected:

    private:
        QTabWidget *m_generalTabWidget;
        QVBoxLayout *m_generalPageLayout;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;
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
#endif // MACHINECONFIGGENERAL_H
