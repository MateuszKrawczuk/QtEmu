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
#include "machineconfiggeneraltabs.h"

class MachineConfigGeneral : public QWidget {
    Q_OBJECT

    public:
        explicit MachineConfigGeneral(Machine *machine,
                                      QWidget *parent = nullptr);
        ~MachineConfigGeneral();

        QWidget *m_generalPageWidget;

        // Methods
        void saveGeneralData();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QTabWidget *m_generalTabWidget;
        QVBoxLayout *m_generalPageLayout;

        Machine *m_machine;
        BasicTab *m_basicTab;
        DescriptionTab *m_descriptionTab;
};

#endif // MACHINECONFIGGENERAL_H
