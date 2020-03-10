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
#include "importmediapage.h"

ImportMediaPage::ImportMediaPage(Machine *machine,
                                 QListWidget *osListWidget,
                                 QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine import wizard"));

    m_infoLabel = new QLabel(tr("Select the media to be imported."));

    this->m_machine = machine;
    this->m_osList = osListWidget;

    QList<QString> header;
    header << tr("Name") << tr("Path");

    m_machineMediaTree = new QTreeWidget(this);
    m_machineMediaTree->setColumnCount(1);
    m_machineMediaTree->setRootIsDecorated(true);
    m_machineMediaTree->setHeaderLabels(header);
    m_machineMediaTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_mediaLayout = new QHBoxLayout();
    m_mediaLayout->addWidget(m_machineMediaTree);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addItem(m_mediaLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ImportMediaPage created";
}

ImportMediaPage::~ImportMediaPage()
{
    qDebug() << "ImportMediaPage destroyed";
}

void ImportMediaPage::initializePage()
{
    QString machineDestinationPath = field("machineDestinationPath").toString();

    QList<Media *> machineMedia = this->m_machine->getMedia();
    for(int i = 0; i < machineMedia.size(); ++i) {
        m_mediaItem = new QTreeWidgetItem(this->m_machineMediaTree, QTreeWidgetItem::Type);
        m_mediaItem->setText(0, machineMedia[i]->name());
        m_mediaItem->setText(1, machineMedia[i]->path());
        m_mediaItem->setData(0, Qt::UserRole, machineMedia[i]->path());

        machineMedia[i]->setPath(QDir::toNativeSeparators(machineDestinationPath));

        QVariant mediaVariant;
        mediaVariant.setValue(machineMedia[i]);

        m_mediaItem->setData(1, Qt::UserRole, mediaVariant);
        m_mediaItem->setCheckState(0, Qt::Checked);
    }
}

bool ImportMediaPage::validatePage()
{
    QString machineDestinationPath = field("machineDestinationPath").toString();
    QString machineConfigFilePath = field("configFilePath").toString();

    QFileInfo machineConfigFileInfo(machineConfigFilePath);

    QString machineConfigFilePathNew =
            QDir::toNativeSeparators(machineDestinationPath + "/" + machineConfigFileInfo.fileName());

    // Move the selected media
    this->m_machine->removeAllMedia();

    bool machineImported = true;

    QTreeWidgetItemIterator it(this->m_machineMediaTree);
    while (*it) {
        if ((*it)->checkState(0) == Qt::Checked) {
            QString oldMediaPath = QDir::toNativeSeparators((*it)->data(0, Qt::UserRole).toString());
            QString newMediaPath = QDir::toNativeSeparators(machineDestinationPath + "/" + (*it)->text(0));

            QVariant mediaVariant = (*it)->data(1, Qt::UserRole);
            Media *media = mediaVariant.value<Media *>();
            media->setPath(newMediaPath);
            media->setUuid(QUuid::createUuid().toString());

            this->m_machine->addMedia(media);

            if (!QFile::copy(oldMediaPath, newMediaPath)) {
                machineImported = false;
                SystemUtils::showMessage(tr("Qtemu - Critical error"),
                                         tr("<p>Cannot import the media: </p>") + media->name(),
                                         QMessageBox::Critical);
            }
        }
        ++it;
    }

    if (!machineImported) {
        return false;
    }

    this->m_machine->setPath(machineDestinationPath);
    this->m_machine->setConfigPath(machineConfigFilePathNew);
    this->m_machine->setUuid(QUuid::createUuid().toString());

    machineImported = this->m_machine->saveMachine();

    // Write the new machine in machines file (the file with all the machines)
    this->m_machine->insertMachineConfigFile();

    this->insertVMList();

    return machineImported;
}

void ImportMediaPage::insertVMList()
{
    QListWidgetItem *machine = new QListWidgetItem(this->m_machine->getName(), this->m_osList);

    machine->setData(QMetaType::QUuid, this->m_machine->getUuid());
    // TODO: Check if the json it's incomplete and the image not exits
    machine->setIcon(QIcon(":/images/os/64x64/" +
                           SystemUtils::getOsIcon(this->m_machine->getOSVersion())));
    this->m_osList->setCurrentItem(machine);
}
