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
#include "exportgeneralpage.h"

ExportGeneralPage::ExportGeneralPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine export wizard"));

    m_infoLabel = new QLabel(tr("Select the folder where the machine has to be exported."));

    m_destinationLineEdit = new QLineEdit();
    m_destinationButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                          "",
                                          this);

    connect(m_destinationButton, &QAbstractButton::clicked,
            this, &ExportGeneralPage::selectExportDestination);

    m_destinationLayout = new QHBoxLayout();
    m_destinationLayout->addWidget(m_destinationLineEdit);
    m_destinationLayout->addWidget(m_destinationButton);

    this->registerField("destination*", m_destinationLineEdit);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addItem(m_destinationLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ExportGeneralPage created";
}

ExportGeneralPage::~ExportGeneralPage()
{
    qDebug() << "ExportGeneralPage destroyed";
}

/**
 * @brief Select the destination of the machine
 *
 * Select the destination folder for the machine
 */
void ExportGeneralPage::selectExportDestination()
{
    QString exportPath = QFileDialog::getExistingDirectory(this, tr("Select the export folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly |
                                                           QFileDialog::DontResolveSymlinks
                                                           );
    if (!exportPath.isEmpty()) {
        this->m_destinationLineEdit->setText(QDir::toNativeSeparators(exportPath));
    }
}
