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

#ifndef EXPORTDETAILSPAGE_H
#define EXPORTDETAILSPAGE_H

// Qt
#include <QWizardPage>
#include <QTreeWidget>
#include <QHBoxLayout>

// Local
#include "../machine.h"

class ExportDetailsPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit ExportDetailsPage(Machine *machine,
                                   QWidget *parent = nullptr);
        ~ExportDetailsPage();

    signals:

    public slots:

    private slots:

    protected:

    private:
        QHBoxLayout *m_mainLayout;

        QTreeWidget *m_machineDetailsTree;

};

#endif // EXPORTDETAILSPAGE_H
