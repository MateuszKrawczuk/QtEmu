/*
 * This file is part of QtEmu project.
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

#ifndef EXPORTMEDIAPAGE_H
#define EXPORTMEDIAPAGE_H

// Qt
#include <QWizardPage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>

// Local
#include "../machine.h"

class ExportMediaPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ExportMediaPage(Machine *machine,
                                 QWidget *parent = nullptr);
        ~ExportMediaPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_mediaLayout;

        QTreeWidget *m_machineMediaTree;
        QTreeWidgetItem *m_mediaItem;

        QLabel *m_mediaTitleLabel;

        Machine *m_machineExport;

        bool validatePage();

};

#endif // EXPORTMEDIAPAGE_H
