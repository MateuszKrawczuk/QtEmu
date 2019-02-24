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
#include "exportmediapage.h"

ExportMediaPage::ExportMediaPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine media"));

    m_mediaTitleLabel = new QLabel(tr("Select the media to be exported"));

    QList<QString> header;
    header << tr("Name") << tr("Path");

    m_machineMediaTree = new QTreeWidget(this);
    m_machineMediaTree->setColumnCount(1);
    m_machineMediaTree->setRootIsDecorated(true);
    m_machineMediaTree->setHeaderLabels(header);
    m_machineMediaTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QList<Media *> machineMedia = machine->getMedia();
    for(int i = 0; i < machineMedia.size(); ++i) {
        m_mediaItem = new QTreeWidgetItem(this->m_machineMediaTree, QTreeWidgetItem::Type);
        m_mediaItem->setText(0, machineMedia[i]->name());
        m_mediaItem->setText(1, machineMedia[i]->path());
        m_mediaItem->setData(0, Qt::UserRole, machineMedia[i]->path());
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
    QString destination = field("destination").toString();

    return true;
}
