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

#ifndef IMPORT_H
#define IMPORT_H

// Qt
#include <QWizard>
#include <QListWidget>

#include <QDebug>

// Local
#include "../machine.h"
#include "importgeneralpage.h"
#include "importdestinationpage.h"
#include "importdetailspage.h"
#include "importmediapage.h"

class ImportWizard : public QWizard {
    Q_OBJECT

    public:
        explicit ImportWizard(Machine *machine,
                              QListWidget *osListWidget,
                              QWidget *parent = nullptr);
        ~ImportWizard();

        enum { Page_General, Page_Destination, Page_Details, Page_Media };

    signals:

    public slots:

    protected:

    private:

};

#endif // IMPORT_H
