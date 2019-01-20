/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#ifndef MACHINECONFIGGENERALTABS_H
#define MACHINECONFIGGENERALTABS_H

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


class BasicTab: public QWidget {
    Q_OBJECT

    public:
        explicit BasicTab(Machine *machine,
                          bool enableFields,
                          QWidget *parent = nullptr);
        ~BasicTab();
        QString getMachineName() const;
        QString getMachineType() const;
        QString getMachineVersion() const;

    signals:

    public slots:

    private slots:
        void selectOS(QString OSSelected);
        QString getStatusLabel(Machine::States state);

    protected:

    private:
        QVBoxLayout *m_basicTabLayout;
        QFormLayout *m_basicTabFormLayout;

        QLineEdit *m_machineNameLineEdit;

        QComboBox *m_OSType;
        QComboBox *m_OSVersion;

        QLabel *m_machineUuidLabel;
        QLabel *m_machineStatusLabel;

        Machine *m_machineConfig;
};

class DescriptionTab: public QWidget {
    Q_OBJECT

    public:
        explicit DescriptionTab(Machine *machine,
                                bool enableFields,
                                QWidget *parent = nullptr);
        ~DescriptionTab();
        QString getMachineDescription() const;

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *m_descriptionLayout;

        QLabel *m_machineDescLabel;
        QPlainTextEdit *m_machineDescTextEdit;

};

#endif // MACHINECONFIGGENERALTABS_H
