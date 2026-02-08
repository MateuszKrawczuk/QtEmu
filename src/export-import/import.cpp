// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
