/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "machineconfigmedia.h"

MachineConfigMedia::MachineConfigMedia(Machine *machine,
                                       QEMU *QEMUGlobalObject,
                                       QWidget *parent) : QWidget(parent)
{

    this->m_machineOptions = machine;
    this->m_qemuGlobalObject = QEMUGlobalObject;

    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    m_mediaPathLabel = new QLabel(this);
    m_mediaSizeLabel = new QLabel(this);
    m_mediaFormatLabel = new QLabel(this);

    m_mediaTree = new QTreeWidget(this);
    m_mediaTree->setEnabled(enableFields);
    m_mediaTree->setMaximumHeight(250);
    m_mediaTree->setMaximumWidth(200);
    m_mediaTree->setColumnCount(1);
    m_mediaTree->setHeaderHidden(true);
    m_mediaTree->setRootIsDecorated(false);
    m_mediaTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    connect(m_mediaTree, &QTreeWidget::itemSelectionChanged,
            this, &MachineConfigMedia::fillDetailsSection);

    m_mediaHash = new QHash<QUuid, Media>;
    QList<Media> machineMedia = machine->getMedia();
    for(int i = 0; i < machineMedia.size(); ++i) {
        QString mediaName;
        mediaName.append("(")
                 .append(machineMedia[i].driveInterface().toUpper())
                 .append(") ")
                 .append(machineMedia[i].name());

        m_mediaItem = new QTreeWidgetItem(this->m_mediaTree, QTreeWidgetItem::Type);
        m_mediaItem->setText(0, mediaName);
        m_mediaItem->setData(0, Qt::UserRole, machineMedia[i].uuid());

        this->m_mediaHash->insert(machineMedia[i].uuid(), machineMedia[i]);
    }

    this->m_mediaTree->setCurrentItem(this->m_mediaTree->itemAt(0, 0));

    m_mediaDetailsLayout = new QFormLayout();
    m_mediaDetailsLayout->setAlignment(Qt::AlignTop);
    m_mediaDetailsLayout->setLabelAlignment(Qt::AlignLeft);
    m_mediaDetailsLayout->setHorizontalSpacing(20);
    m_mediaDetailsLayout->setVerticalSpacing(10);
    m_mediaDetailsLayout->addRow(tr("Path") + ":", m_mediaPathLabel);
    m_mediaDetailsLayout->addRow(tr("Size") + ":", m_mediaSizeLabel);
    m_mediaDetailsLayout->addRow(tr("Format") + ":", m_mediaFormatLabel);

    m_mediaSettingsGroupBox = new QGroupBox(tr("Details"), this);
    m_mediaSettingsGroupBox->setLayout(m_mediaDetailsLayout);

    m_cacheComboBox = new QComboBox(this);
    m_cacheComboBox->setEnabled(enableFields);
    m_cacheComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_cacheComboBox->addItem("none");
    m_cacheComboBox->addItem("writethrough");
    m_cacheComboBox->addItem("writeback");
    m_cacheComboBox->addItem("directsync");
    m_cacheComboBox->addItem("unsafe");
    m_cacheComboBox->setCurrentIndex(0);

    m_IOComboBox = new QComboBox(this);
    m_IOComboBox->setEnabled(enableFields);
    m_IOComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_IOComboBox->addItem("threads");
    m_IOComboBox->addItem("native");
    m_IOComboBox->setCurrentIndex(0);

    m_readOnlyMediaCheck = new QCheckBox(this);
    m_readOnlyMediaCheck->setEnabled(enableFields);

    m_mediaOptionsLayout = new QFormLayout();
    m_mediaOptionsLayout->setAlignment(Qt::AlignTop);
    m_mediaOptionsLayout->setLabelAlignment(Qt::AlignLeft);
    m_mediaOptionsLayout->setHorizontalSpacing(20);
    m_mediaOptionsLayout->setVerticalSpacing(10);
    m_mediaOptionsLayout->addRow(tr("Cache mode") + ":", m_cacheComboBox);
    m_mediaOptionsLayout->addRow(tr("IO mode") + ":", m_IOComboBox);
    m_mediaOptionsLayout->addRow(tr("Read only") + ":", m_readOnlyMediaCheck);

    m_mediaOptionsGroupBox = new QGroupBox(tr("Options"), this);
    m_mediaOptionsGroupBox->setFlat(true);
    m_mediaOptionsGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mediaOptionsGroupBox->setLayout(m_mediaOptionsLayout);

    m_addFloppyPushButton = new QPushButton(this);
    m_addFloppyPushButton->setEnabled(enableFields);
    m_addFloppyPushButton->setIcon(QIcon::fromTheme("media-floppy",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));
    m_addFloppyPushButton->setToolTip(tr("Add Floppy"));

    connect(m_addFloppyPushButton, &QAbstractButton::clicked,
            this, &MachineConfigMedia::addFloppyMedia);

    m_addHDDPushButton = new QPushButton(this);
    m_addHDDPushButton->setEnabled(enableFields);
    m_addHDDPushButton->setIcon(QIcon::fromTheme("drive-harddisk",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));
    m_addHDDPushButton->setToolTip(tr("Add hard disk media"));

    connect(m_addHDDPushButton, &QAbstractButton::clicked,
            this, &MachineConfigMedia::addHddMedia);

    m_addCDROMPushButton = new QPushButton(this);
    m_addCDROMPushButton->setEnabled(enableFields);
    m_addCDROMPushButton->setIcon(QIcon::fromTheme("media-optical-data",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));
    m_addCDROMPushButton->setToolTip(tr("Add optical media"));

    connect(m_addCDROMPushButton, &QAbstractButton::clicked,
            this, &MachineConfigMedia::addOpticalMedia);

    m_mediaAddLayout = new QHBoxLayout();
    m_mediaAddLayout->setAlignment(Qt::AlignTop);
    m_mediaAddLayout->addWidget(m_addFloppyPushButton);
    m_mediaAddLayout->addWidget(m_addHDDPushButton);
    m_mediaAddLayout->addWidget(m_addCDROMPushButton);

    m_mediaAddGroupBox = new QGroupBox();
    m_mediaAddGroupBox->setLayout(m_mediaAddLayout);
    m_mediaAddGroupBox->setFlat(true);

    m_mediaPageLayout = new QGridLayout();
    m_mediaPageLayout->setAlignment(Qt::AlignTop);
    m_mediaPageLayout->setSpacing(1);
    m_mediaPageLayout->addWidget(m_mediaTree,             0, 0, 1, 1);
    m_mediaPageLayout->addWidget(m_mediaSettingsGroupBox, 0, 1, 1, 1);
    m_mediaPageLayout->addWidget(m_mediaAddGroupBox,      1, 0, 1, 1);
    //m_mediaPageLayout->addWidget(m_mediaOptionsGroupBox,  1, 1, 1, 1); // TODO: In QtEmu 2.1

    m_mediaPageWidget = new QWidget();
    m_mediaPageWidget->setLayout(m_mediaPageLayout);

    qDebug() << "MachineConfigMedia created";
}

MachineConfigMedia::~MachineConfigMedia()
{
    qDebug() << "MachineConfigMedia destroyed";
}

void MachineConfigMedia::fillDetailsSection()
{
    QUuid selectedMediaUuid = this->m_mediaTree->currentItem()->data(0, Qt::UserRole).toUuid();
    this->m_mediaPathLabel->setText(this->m_mediaHash->value(selectedMediaUuid).path());
    this->m_mediaSizeLabel->setText(QString::number(this->m_mediaHash->value(selectedMediaUuid).size()));
    this->m_mediaFormatLabel->setText(this->m_mediaHash->value(selectedMediaUuid).format());
}

void MachineConfigMedia::addFloppyMedia()
{

}

void MachineConfigMedia::addHddMedia()
{
    m_addHddDiskMessageBox = new QMessageBox(this);
    m_addHddDiskMessageBox->setWindowTitle(tr("QtEmu - Add Hard Disk"));
    m_addHddDiskMessageBox->setIcon(QMessageBox::Question);
    m_addHddDiskMessageBox->setText(tr("<p>Add a new Hard Disk</p>"));

    QPushButton *newDiskButton = m_addHddDiskMessageBox->addButton(tr("Create new disk"), QMessageBox::ActionRole);
    QPushButton *existingDiskButton = m_addHddDiskMessageBox->addButton(tr("Choose existing disk"), QMessageBox::ActionRole);
    QPushButton *cancelButton = m_addHddDiskMessageBox->addButton(tr("Exit"), QMessageBox::ActionRole);

    m_addHddDiskMessageBox->exec();

    QString diskPath;

    if (m_addHddDiskMessageBox->clickedButton() == newDiskButton) {
        NewDiskWizard newDiskWizard(this->m_machineOptions, this->m_qemuGlobalObject, this);

        newDiskWizard.show();
        newDiskWizard.exec();

    } else if (m_addHddDiskMessageBox->clickedButton() == existingDiskButton) {
       diskPath = QFileDialog::getOpenFileName(this, tr("Select disk"), QDir::homePath());
    } else if (m_addHddDiskMessageBox->clickedButton() == cancelButton) {
        m_addHddDiskMessageBox->close();
    }
}

void MachineConfigMedia::addOpticalMedia()
{

}
