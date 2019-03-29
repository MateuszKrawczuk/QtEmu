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

// Local
#include "import.h"

ImportWizard::ImportWizard(Machine *machine,
                           QListWidget *osListWidget,
                           QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("Import the Machine"));

    this->setPage(Page_General, new ImportGeneralPage(this));
    this->setPage(Page_Destination, new ImportDestinationPage(this));
    this->setPage(Page_Details, new ImportDetailsPage(machine, this));
    this->setPage(Page_Media, new ImportMediaPage(machine, osListWidget, this));

    this->setStartId(Page_General);

    #ifndef Q_OS_MAC
        this->setWizardStyle(ClassicStyle);
    #endif
    #ifdef Q_OS_MAC
        this->setWizardStyle(MacStyle);
    #endif

    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    this->setMinimumSize(600, 400);

    qDebug() << "ImportWizard created";
}

ImportWizard::~ImportWizard()
{
    qDebug() << "ImportWizard destroyed";
}
