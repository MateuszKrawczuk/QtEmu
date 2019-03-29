/*
 * This file is part of QtEmu project.
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

#ifndef IMPORTMEDIAPAGE_H
#define IMPORTMEDIAPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>

#include <QDebug>

// Local
#include "../machine.h"

class ImportMediaPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ImportMediaPage(Machine *machine,
                                 QListWidget *osListWidget,
                                 QWidget *parent = nullptr);
        ~ImportMediaPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_mediaLayout;
        QVBoxLayout *m_mainLayout;

        QTreeWidget *m_machineMediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_infoLabel;

        QListWidget *m_osList;

        Machine *m_machine;

        // Methods
        void initializePage();
        bool validatePage();
        void insertVMList();
};

#endif // IMPORTMEDIAPAGE_H
