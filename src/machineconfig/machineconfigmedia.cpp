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

/**
 * @brief Configuration of the machine. Media page
 * @param machine, machine to be configured
 * @param QEMUGlobalObject, QEMU global object with data about QEMU
 * @param parent, parent widget
 *
 * Configuration of the machine. Media page.
 * In this page you can add or remove hdd, cdrom and floppy
 * to the machine
 */
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

    m_mediaNameLabel = new QLabel(this);
    m_mediaNameLabel->setWordWrap(true);
    m_mediaPathLabel = new QLabel(this);
    m_mediaPathLabel->setWordWrap(true);

    m_mediaTree = new QTreeWidget(this);
    m_mediaTree->setEnabled(enableFields);
    m_mediaTree->setMaximumHeight(250);
    m_mediaTree->setMaximumWidth(200);
    m_mediaTree->setColumnCount(1);
    m_mediaTree->setHeaderHidden(true);
    m_mediaTree->setRootIsDecorated(false);
    m_mediaTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_mediaTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_mediaTree, &QTreeWidget::itemPressed,
            this, &MachineConfigMedia::fillDetailsSection);
    connect(m_mediaTree, &QTreeWidget::customContextMenuRequested,
            this, &MachineConfigMedia::removeMediaMenu);

    this->fillMaps();

    QList<Media *> machineMedia = machine->getMedia();
    for(int i = 0; i < machineMedia.size(); ++i) {
        this->addMediaToTree(machineMedia[i]);
    }
    this->m_mediaTree->setCurrentItem(this->m_mediaTree->itemAt(0, 0));
    this->fillDetailsSection();

    m_removeMediaAction = new QAction(QIcon::fromTheme("remove",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/remove.svg"))),
                                      tr("Remove media"),
                                      this);
    connect(m_removeMediaAction, &QAction::triggered,
            this, &MachineConfigMedia::removeMediaFromTree);

    m_menu = new QMenu(this);
    m_menu->addAction(m_removeMediaAction);

    m_mediaDetailsLayout = new QFormLayout();
    m_mediaDetailsLayout->setAlignment(Qt::AlignTop);
    m_mediaDetailsLayout->setLabelAlignment(Qt::AlignLeft);
    //m_mediaDetailsLayout->setHorizontalSpacing(20);
    //m_mediaDetailsLayout->setVerticalSpacing(10);
    m_mediaDetailsLayout->addRow(tr("Name") + ":", m_mediaNameLabel);
    m_mediaDetailsLayout->addRow(tr("Path") + ":", m_mediaPathLabel);

    // QtEmu 2.1
    m_mediaSettingsGroupBox = new QGroupBox(tr("Details"), this);
    m_mediaSettingsGroupBox->setLayout(m_mediaDetailsLayout);

    /*m_cacheComboBox = new QComboBox(this);
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
    m_mediaOptionsGroupBox->setLayout(m_mediaOptionsLayout);*/

    m_addFloppyPushButton = new QPushButton(this);
    m_addFloppyPushButton->setEnabled(enableFields);
    m_addFloppyPushButton->setIcon(QIcon::fromTheme("media-floppy",
                                                    QIcon(QPixmap(":/images/icons/breeze/32x32/document-save.svg"))));
    m_addFloppyPushButton->setToolTip(tr("Add Floppy"));

    connect(m_addFloppyPushButton, &QAbstractButton::clicked,
            this, &MachineConfigMedia::addFloppyMedia);

    m_addHDDPushButton = new QPushButton(this);
    m_addHDDPushButton->setEnabled(enableFields);
    m_addHDDPushButton->setIcon(QIcon::fromTheme("drive-harddisk-root",
                                                 QIcon(QPixmap(":/images/icons/breeze/32x32/drive-harddisk.svg"))));
    m_addHDDPushButton->setToolTip(tr("Add hard disk media"));

    connect(m_addHDDPushButton, &QAbstractButton::clicked,
            this, &MachineConfigMedia::addHddMedia);

    m_addCDROMPushButton = new QPushButton(this);
    m_addCDROMPushButton->setEnabled(enableFields);
    m_addCDROMPushButton->setIcon(QIcon::fromTheme("media-optical-data",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/media-optical-data.svg"))));
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

/**
 * @brief Remove the selected media
 *
 * Remove the selected media
 */
void MachineConfigMedia::removeMediaMenu(const QPoint &pos)
{
    this->m_menu->exec(this->m_mediaTree->mapToGlobal(pos));
}

/**
 * @brief Fill the details section
 *
 * Fill the details section
 */
void MachineConfigMedia::fillDetailsSection()
{
    if (this->countMedia() <= 0) {
        this->m_mediaNameLabel->setText("");
        this->m_mediaPathLabel->setText("");
        return;
    }

    QVariant mediaVariant = this->m_mediaTree->currentItem()->data(0, Qt::UserRole);
    Media *selectedMedia = mediaVariant.value<Media *>();

    this->m_mediaNameLabel->setText(selectedMedia->name());
    this->m_mediaPathLabel->setText(selectedMedia->path());
}

/**
 * @brief Add a floppy
 *
 * Add a floppy to the media list.
 * Max two floppy per machine, fda and fdb
 */
void MachineConfigMedia::addFloppyMedia()
{
    if (this->m_floppyMap->size() == 0) {
        m_maxFloppyMessageBox = new QMessageBox();
        m_maxFloppyMessageBox->setWindowTitle(tr("Qtemu - floppy"));
        m_maxFloppyMessageBox->setIcon(QMessageBox::Critical);
        m_maxFloppyMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_maxFloppyMessageBox->setText(tr("<p>Maximum number of floppy reached</p>"));
        m_maxFloppyMessageBox->exec();
        return;
    }

    QString floppyPath = QFileDialog::getOpenFileName(this, tr("Select disk"), QDir::homePath());

    if (floppyPath.isEmpty()) {
        return;
    }

    QFileInfo floppyInfo(floppyPath);

    Media *media = new Media(this->m_machineOptions);
    media->setName(floppyInfo.fileName());
    media->setPath(QDir::toNativeSeparators(floppyInfo.absoluteFilePath()));
    media->setType("fdd");
    media->setDriveInterface(this->m_floppyMap->first());
    media->setUuid(QUuid::createUuid().toString());

    this->addMediaToTree(media);
    this->fillDetailsSection();
}

/**
 * @brief Add hdd media
 *
 * Add hdd media to the media list.
 * hda, hdb, hdc and hdb can be addeds
 */
void MachineConfigMedia::addHddMedia()
{
    if (this->m_diskMap->size() == 0) {
        m_maxHddDiskMessageBox = new QMessageBox();
        m_maxHddDiskMessageBox->setWindowTitle(tr("Qtemu - hard disk"));
        m_maxHddDiskMessageBox->setIcon(QMessageBox::Critical);
        m_maxHddDiskMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_maxHddDiskMessageBox->setText(tr("<p>Maximum number of hard disks reached</p>"));
        m_maxHddDiskMessageBox->exec();
        return;
    }

    m_addHddDiskMessageBox = new QMessageBox(this);
    m_addHddDiskMessageBox->setWindowTitle(tr("QtEmu - Add Hard Disk"));
    m_addHddDiskMessageBox->setIcon(QMessageBox::Question);
    m_addHddDiskMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    m_addHddDiskMessageBox->setText(tr("<p>Add a new Hard Disk</p>"));

    QPushButton *newDiskButton = m_addHddDiskMessageBox->addButton(tr("Create new disk"),
                                                                   QMessageBox::ActionRole);
    QPushButton *existingDiskButton = m_addHddDiskMessageBox->addButton(tr("Choose existing disk"),
                                                                        QMessageBox::ActionRole);
    QPushButton *cancelButton = m_addHddDiskMessageBox->addButton(tr("Exit"),
                                                                  QMessageBox::ActionRole);

    m_addHddDiskMessageBox->exec();

    if (m_addHddDiskMessageBox->clickedButton() == newDiskButton) {
        Media *newMedia = new Media(this->m_machineOptions);
        newMedia->setDriveInterface(this->m_diskMap->first());

        NewDiskWizard newDiskWizard(this->m_machineOptions, this->m_qemuGlobalObject, newMedia, this);

        newDiskWizard.show();
        newDiskWizard.exec();

        if (!newMedia->name().isEmpty() && !newMedia->path().isEmpty()){
            this->addMediaToTree(newMedia);
        }
    } else if (m_addHddDiskMessageBox->clickedButton() == existingDiskButton) {
       QString diskPath = QFileDialog::getOpenFileName(this, tr("Select disk"), QDir::homePath());

       if (diskPath.isEmpty()) {
           return;
       }

       QFileInfo hddInfo(diskPath);

       Media *existingMedia = new Media(this->m_machineOptions);
       existingMedia->setName(hddInfo.fileName());
       existingMedia->setPath(QDir::toNativeSeparators(hddInfo.absoluteFilePath()));
       existingMedia->setType("hdd");
       existingMedia->setDriveInterface(this->m_diskMap->first());
       existingMedia->setUuid(QUuid::createUuid().toString());

       this->addMediaToTree(existingMedia);

    } else if (m_addHddDiskMessageBox->clickedButton() == cancelButton) {
        m_addHddDiskMessageBox->close();
    }
    this->fillDetailsSection();
}

/**
 * @brief Add a cdrom
 *
 * Add a cdrom to the media list.
 * If the hdc is added to the media list.
 * A cdrom can't be added
 */
void MachineConfigMedia::addOpticalMedia()
{
    if (this->m_cdromMap->size() == 0) {
        m_maxOpticalMessageBox = new QMessageBox();
        m_maxOpticalMessageBox->setWindowTitle(tr("Qtemu - cdrom"));
        m_maxOpticalMessageBox->setIcon(QMessageBox::Critical);
        m_maxOpticalMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_maxOpticalMessageBox->setText(tr("<p>Only one cdrom can be added</p>"));
        m_maxOpticalMessageBox->exec();
        return;
    }

    if (!this->m_diskMap->contains("hdc")) {
        m_hdcOpticalConflictMessageBox = new QMessageBox();
        m_hdcOpticalConflictMessageBox->setWindowTitle(tr("Qtemu - cdrom"));
        m_hdcOpticalConflictMessageBox->setIcon(QMessageBox::Critical);
        m_hdcOpticalConflictMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_hdcOpticalConflictMessageBox->setText(tr("<p>You cannot use hdc and cdrom at the same time</p>"));
        m_hdcOpticalConflictMessageBox->exec();
        return;
    }

    QString cdromPath = QFileDialog::getOpenFileName(this, tr("Select cdrom"), QDir::homePath());

    if (cdromPath.isEmpty()) {
        return;
    }

    QFileInfo cdromInfo(cdromPath);

    Media *media = new Media(this->m_machineOptions);
    media->setName(cdromInfo.fileName());
    media->setPath(QDir::toNativeSeparators(cdromInfo.absoluteFilePath()));
    media->setType("cdrom");
    media->setDriveInterface(this->m_cdromMap->first());
    media->setUuid(QUuid::createUuid().toString());

    this->removeInterface("hdc");
    this->addMediaToTree(media);
    this->fillDetailsSection();
}

/**
 * @brief Fill the Aux maps
 *
 * Fill the diskMap and floppyMap
 */
void MachineConfigMedia::fillMaps()
{
    this->m_diskMap = new QMap<QString, QString>;
    this->m_diskMap->insert("hda", "hda");
    this->m_diskMap->insert("hdb", "hdb");
    this->m_diskMap->insert("hdc", "hdc");
    this->m_diskMap->insert("hdd", "hdd");

    this->m_floppyMap = new QMap<QString, QString>;
    this->m_floppyMap->insert("fda", "fda");
    this->m_floppyMap->insert("fdb", "fdb");

    this->m_cdromMap = new QMap<QString, QString>;
    this->m_cdromMap->insert("cdrom", "cdrom");
}

/**
 * @brief Add the media to the tree
 * @param media, machine media
 *
 * Add the media to the tree and removes
 * all the interfaces
 */
void MachineConfigMedia::addMediaToTree(Media *media)
{
    QString mediaName;
    mediaName.append("(")
             .append(media->driveInterface().toUpper())
             .append(") ")
             .append(media->name());

    QVariant mediaVariant;
    mediaVariant.setValue(media);

    m_mediaItem = new QTreeWidgetItem(this->m_mediaTree, QTreeWidgetItem::Type);
    m_mediaItem->setText(0, mediaName);
    m_mediaItem->setData(0, Qt::UserRole, mediaVariant);

    // To prevent undefined behavior :'(
    this->m_mediaTree->setCurrentItem(m_mediaItem);
    this->removeInterface(media->driveInterface());
}

/**
 * @brief Remove the selected media from the tree
 *
 * Remove the selected media from the tree
 */
void MachineConfigMedia::removeMediaFromTree()
{
    QVariant mediaVariant = this->m_mediaTree->currentItem()->data(0, Qt::UserRole);
    Media *selectedMedia = mediaVariant.value<Media *>();

    this->addInterface(selectedMedia->driveInterface());

    this->m_mediaTree->removeItemWidget(this->m_mediaTree->currentItem(), 0);
    delete this->m_mediaTree->currentItem();
    this->fillDetailsSection();
}

/**
 * @brief Add the interface to the maps
 * @param driveInterface, interface to be added
 *
 * Add the interface to the maps
 */
void MachineConfigMedia::addInterface(const QString driveInterface)
{
    if (driveInterface == "cdrom") {
        this->m_cdromMap->insert(driveInterface, driveInterface);
    } else if (driveInterface.contains("hd")) {
        this->m_diskMap->insert(driveInterface, driveInterface);
    } else if (driveInterface.contains("fd")) {
        this->m_floppyMap->insert(driveInterface, driveInterface);
    }
}

/**
 * @brief Remove the interface from the maps
 * @param driveInterface, interface to be removed
 *
 * Remove the interface from the maps
 */
void MachineConfigMedia::removeInterface(const QString driveInterface)
{
    this->m_diskMap->remove(driveInterface);
    this->m_floppyMap->remove(driveInterface);
    this->m_cdromMap->remove(driveInterface);
}

/**
 * @brief Count the number of elements
 * @return number of items
 *
 * Count the number of media that the m_mediaTree have
 * inserted
 */
int MachineConfigMedia::countMedia()
{
    int count = 0;
    QTreeWidgetItemIterator it(this->m_mediaTree);
    while (*it) {
        ++count;
        ++it;
    }
    return count;
}

/**
 * @brief Save the media
 *
 * Add all the media to the machine
 */
void MachineConfigMedia::saveMediaData()
{
    // Remove all media from the machine
    this->m_machineOptions->removeAllMedia();

    QTreeWidgetItemIterator it(this->m_mediaTree);
    while (*it) {
        QVariant mediaVariant = (*it)->data(0, Qt::UserRole);
        Media *media = mediaVariant.value<Media *>();
        this->m_machineOptions->addMedia(media);
        ++it;
    }
}
