// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "export.h"

ExportWizard::ExportWizard(Machine *machine,
                           QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("Export the Machine"));

    this->setPage(Page_General, new ExportGeneralPage(this));
    this->setPage(Page_Details, new ExportDetailsPage(machine, this));
    this->setPage(Page_Media, new ExportMediaPage(machine, this));

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

    qDebug() << "ExportWizard created";
}

ExportWizard::~ExportWizard()
{
    qDebug() << "ExportWizard destroyed";
}
