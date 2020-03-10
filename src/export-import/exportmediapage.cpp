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

// Local
#include "exportmediapage.h"

ExportMediaPage::ExportMediaPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine media"));

    this->m_machineExport = machine;

    m_mediaTitleLabel = new QLabel(tr("Select the media to be exported."));

    QList<QString> header;
    header << tr("Name") << tr("Path");

    m_machineMediaTree = new QTreeWidget(this);
    m_machineMediaTree->setColumnCount(1);
    m_machineMediaTree->setRootIsDecorated(true);
    m_machineMediaTree->setHeaderLabels(header);
    m_machineMediaTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QString destination = field("destination").toString();

    QList<Media *> machineMedia = machine->getMedia();
    for(int i = 0; i < machineMedia.size(); ++i) {
        m_mediaItem = new QTreeWidgetItem(this->m_machineMediaTree, QTreeWidgetItem::Type);
        m_mediaItem->setText(0, machineMedia[i]->name());
        m_mediaItem->setText(1, machineMedia[i]->path());
        m_mediaItem->setData(0, Qt::UserRole, machineMedia[i]->path());

        machineMedia[i]->setPath(QDir::toNativeSeparators(destination));

        QVariant mediaVariant;
        mediaVariant.setValue(machineMedia[i]);

        m_mediaItem->setData(1, Qt::UserRole, mediaVariant);
        m_mediaItem->setCheckState(0, Qt::Checked);
    }

    m_mediaLayout = new QHBoxLayout();
    m_mediaLayout->addWidget(m_machineMediaTree);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addWidget(m_mediaTitleLabel);
    m_mainLayout->addItem(m_mediaLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ExportMediaPage created";
}

ExportMediaPage::~ExportMediaPage()
{
    qDebug() << "ExportMediaPage destroyed";
}

bool ExportMediaPage::validatePage()
{
    QString machineDestinationPath = field("destination").toString();

    this->m_machineExport->removeAllMedia();

    QTreeWidgetItemIterator it(this->m_machineMediaTree);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked) {
            QString oldMediaPath = QDir::toNativeSeparators((*it)->data(0, Qt::UserRole).toString());
            QString newMediaPath = QDir::toNativeSeparators(machineDestinationPath + "/" + (*it)->text(0));

            QVariant mediaVariant = (*it)->data(1, Qt::UserRole);
            Media *media = mediaVariant.value<Media *>();
            media->setPath(newMediaPath);

            this->m_machineExport->addMedia(media);

            if (!QFile::copy(oldMediaPath, newMediaPath)) {
                SystemUtils::showMessage(tr("Qtemu - Critical error"),
                                         tr("<p>Cannot export the media: </p>") + media->name(),
                                         QMessageBox::Critical);
            }
        }
        ++it;
    }

    QString machineDestionation =
            QDir::toNativeSeparators(machineDestinationPath + "/" + this->m_machineExport->getName().toLower().replace(" ", "_") + ".json");

    this->m_machineExport->setPath(machineDestinationPath);
    this->m_machineExport->setConfigPath(machineDestionation);
    this->m_machineExport->saveMachine();

    return true;
}
