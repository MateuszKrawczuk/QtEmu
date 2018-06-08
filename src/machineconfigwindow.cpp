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

// Qt

// Local
#include "machineconfigwindow.h"

MachineConfigWindow::MachineConfigWindow(QUuid machineUuid,
                                         Machine::States machineState,
                                         QWidget *parent) : QWidget(parent) {

    this -> setWindowTitle(tr("Machine Preferences") + " - QtEmu");
    this -> setWindowIcon(QIcon::fromTheme("qtemu",
                                           QIcon(":/images/qtemu.png")));
    this -> setWindowFlags(Qt::Dialog);
    this -> setWindowModality(Qt::ApplicationModal);
    this -> setMinimumSize(640, 500);

    this -> createGeneralPage();
    this -> createHardwarePage();
    this -> createBootPage();
    this -> createMediaPage();

    m_optionsListWidget = new QListWidget(this);
    m_optionsListWidget -> setViewMode(QListView::ListMode);
    m_optionsListWidget -> setIconSize(QSize(32, 32));
    m_optionsListWidget -> setMovement(QListView::Static);
    m_optionsListWidget -> setMaximumWidth(170);
    m_optionsListWidget -> setSpacing(7);
    m_optionsListWidget -> setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add options
    m_optionsListWidget -> addItem(tr("General"));
    m_optionsListWidget -> item(0) -> setIcon(QIcon::fromTheme("preferences-plugin",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));

    m_optionsListWidget -> addItem(tr("Hardware"));
    m_optionsListWidget -> item(1) -> setIcon(QIcon::fromTheme("cpu",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/update-none.svg"))));

    m_optionsListWidget -> addItem(tr("Boot options"));
    m_optionsListWidget -> item(2) -> setIcon(QIcon::fromTheme("checkmark",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/applications-education-language.svg"))));

    m_optionsListWidget -> addItem(tr("Media"));
    m_optionsListWidget -> item(3) -> setIcon(QIcon::fromTheme("ntfs-config",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/practice-start.svg"))));

    m_optionsListWidget -> addItem(tr("Network"));
    m_optionsListWidget -> item(4) -> setIcon(QIcon::fromTheme("network-card",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    m_optionsListWidget -> addItem(tr("Audio"));
    m_optionsListWidget -> item(5) -> setIcon(QIcon::fromTheme("audio-card",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));// TODO

    m_optionsListWidget -> addItem(tr("Accelerator"));
    m_optionsListWidget -> item(6) -> setIcon(QIcon::fromTheme("mathematica",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    m_optionsListWidget -> addItem(tr("Display"));
    m_optionsListWidget -> item(7) -> setIcon(QIcon::fromTheme("applications-multimedia",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/network-manager.svg"))));

    // Prepare window
    m_optionsStackedWidget = new QStackedWidget(this);
    m_optionsStackedWidget -> setSizePolicy(QSizePolicy::Expanding,
                                            QSizePolicy::Expanding);
    m_optionsStackedWidget -> addWidget(this -> m_generalPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_hardwarePageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_bootPageWidget);
    m_optionsStackedWidget -> addWidget(this -> m_mediaPageWidget);

    connect(m_optionsListWidget, &QListWidget::currentRowChanged,
            m_optionsStackedWidget, &QStackedWidget::setCurrentIndex);

    m_topLayout = new QHBoxLayout();
    m_topLayout -> addWidget(m_optionsListWidget);
    m_topLayout -> addWidget(m_optionsStackedWidget);

    // Buttons
    m_saveButton = new QPushButton(QIcon::fromTheme("document-save",
                                                    QIcon(QPixmap(":/images/icons/breeze/32x32/document-save.svg"))),
                                   tr("Save"),
                                   this);
    connect(m_saveButton, &QAbstractButton::clicked,
            this, &MachineConfigWindow::saveMachineSettings);

    m_closeButton = new QPushButton(QIcon::fromTheme("dialog-cancel",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))),
                                    tr("Cancel"),
                                    this);
    connect(m_closeButton, &QAbstractButton::clicked,
            this, &QWidget::hide);

    this -> m_buttonsLayout = new QHBoxLayout();
    m_buttonsLayout -> setAlignment(Qt::AlignRight);
    m_buttonsLayout -> addWidget(m_saveButton);
    m_buttonsLayout -> addWidget(m_closeButton);

    m_closeAction = new QAction(this);
    m_closeAction -> setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_closeAction, &QAction::triggered, this, &QWidget::hide);
    this -> addAction(m_closeAction);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout -> addLayout(m_topLayout, 20);
    m_mainLayout -> addLayout(m_buttonsLayout);

    this -> setLayout(m_mainLayout);

    this -> m_optionsListWidget -> setCurrentRow(0);
    this -> m_optionsListWidget -> setFocus();

    qDebug() << "MachineConfigWindow created";

}

MachineConfigWindow::~MachineConfigWindow() {
    qDebug() << "MachineConfigWindow destroyed";
}

void MachineConfigWindow::createGeneralPage() {

    Machine *machine = new Machine();

    m_generalTabWidget = new QTabWidget();
    m_generalTabWidget -> setSizePolicy(QSizePolicy::MinimumExpanding,
                                        QSizePolicy::MinimumExpanding);
    m_generalTabWidget -> addTab(new BasicTab(machine, this), tr("Basic Details"));
    m_generalTabWidget -> addTab(new DescriptionTab(machine, this), tr("Description"));

    m_generalPageLayout = new QVBoxLayout();
    m_generalPageLayout -> setAlignment(Qt::AlignCenter);
    m_generalPageLayout -> addWidget(m_generalTabWidget);

    m_generalPageWidget = new QWidget(this);
    m_generalPageWidget -> setLayout(m_generalPageLayout);

    qDebug() << "General page created";
}

BasicTab::BasicTab(Machine *machine,
                   QWidget *parent) : QWidget(parent) {

    m_machineNameLineEdit = new QLineEdit();

    m_OSType = new QComboBox();
    m_OSType -> setSizePolicy(QSizePolicy::Expanding,
                              QSizePolicy::MinimumExpanding);

    m_OSType -> addItem("GNU/Linux");
    m_OSType -> addItem("Microsoft Windows");
    m_OSType -> addItem("BSD");
    m_OSType -> addItem(tr("Other"));

    connect(m_OSType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
           this, &BasicTab::selectOS);

    m_OSVersion = new QComboBox();
    m_OSVersion -> setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::MinimumExpanding);

    this -> selectOS(0);

    m_machineUuidLabel = new QLabel();
    m_machineStatusLabel = new QLabel();

    m_basicTabFormLayout = new QFormLayout();
    m_basicTabFormLayout -> setAlignment(Qt::AlignTop);
    m_basicTabFormLayout -> setLabelAlignment(Qt::AlignLeft);
    m_basicTabFormLayout -> setHorizontalSpacing(20);
    m_basicTabFormLayout -> setVerticalSpacing(10);
    m_basicTabFormLayout -> addRow(tr("Name") + ":", m_machineNameLineEdit);
    m_basicTabFormLayout -> addRow(tr("Type") + ":", m_OSType);
    m_basicTabFormLayout -> addRow(tr("Version") + ":", m_OSVersion);
    m_basicTabFormLayout -> addRow(tr("UUID") + ":", m_machineUuidLabel);
    m_basicTabFormLayout -> addRow(tr("Status") + ":", m_machineStatusLabel);

    m_basicTabLayout = new QVBoxLayout();
    m_basicTabLayout -> addItem(m_basicTabFormLayout);

    this -> setLayout(m_basicTabLayout);

    qDebug() << "BasicTab created";
}

BasicTab::~BasicTab() {
    qDebug() << "BasicTab destroyed";
}

void BasicTab::selectOS(int OSSelected){

    this -> m_OSVersion -> clear();

    if (OSSelected == 0) {
        this -> m_OSVersion -> addItem(tr("Debian"));
        this -> m_OSVersion -> addItem(tr("Ubuntu"));
        this -> m_OSVersion -> addItem(tr("Fedora"));
        this -> m_OSVersion -> addItem(tr("OpenSuse"));
        this -> m_OSVersion -> addItem(tr("Mageia"));
        this -> m_OSVersion -> addItem(tr("Gentoo"));
        this -> m_OSVersion -> addItem(tr("Arch Linux"));
        this -> m_OSVersion -> addItem(tr("Linux"));
    } else if (OSSelected == 1) {
        this -> m_OSVersion -> addItem(tr("Microsoft 95"));
        this -> m_OSVersion -> addItem(tr("Microsoft 98"));
        this -> m_OSVersion -> addItem(tr("Microsoft 2000"));
        this -> m_OSVersion -> addItem(tr("Microsoft XP"));
        this -> m_OSVersion -> addItem(tr("Microsoft Vista"));
        this -> m_OSVersion -> addItem(tr("Microsoft 7"));
        this -> m_OSVersion -> addItem(tr("Microsoft 8"));
        this -> m_OSVersion -> addItem(tr("Microsoft 10"));
    } else if (OSSelected == 2) {
        this -> m_OSVersion -> addItem(tr("FreeBSD"));
        this -> m_OSVersion -> addItem(tr("OpenBSD"));
        this -> m_OSVersion -> addItem(tr("NetBSD"));
    } else if (OSSelected == 3) {
        this -> m_OSVersion -> addItem(tr("Debian GNU Hurd"));
        this -> m_OSVersion -> addItem(tr("Arch Hurd"));
        this -> m_OSVersion -> addItem(tr("Redox"));
        this -> m_OSVersion -> addItem(tr("ReactOS"));
    }
}

DescriptionTab::DescriptionTab(Machine *machine,
                               QWidget *parent) : QWidget(parent) {

    m_machineDescLabel = new QLabel(tr("Description") + ":");
    m_machineDescTextEdit = new QPlainTextEdit();

    m_descriptionLayout = new QVBoxLayout();
    m_descriptionLayout -> setAlignment(Qt::AlignVCenter);
    m_descriptionLayout -> addWidget(m_machineDescLabel);
    m_descriptionLayout -> addWidget(m_machineDescTextEdit);

    this -> setLayout(m_descriptionLayout);

    qDebug() << "Created destroyed";
}

DescriptionTab::~DescriptionTab() {
    qDebug() << "DescriptionTab destroyed";
}

void MachineConfigWindow::createHardwarePage() {

    // TODO: Change this machine to the selected machine
    Machine *machine = new Machine();

    m_hardwareTabWidget = new QTabWidget();
    m_hardwareTabWidget -> setSizePolicy(QSizePolicy::MinimumExpanding,
                                         QSizePolicy::MinimumExpanding);
    m_hardwareTabWidget -> addTab(new ProcessorTab(machine, Qt::AlignTop, this), tr("CPU"));
    m_hardwareTabWidget -> addTab(new GraphicsTab(machine, this), tr("Graphics"));

    m_hardwarePageLayout = new QVBoxLayout();
    m_hardwarePageLayout -> setAlignment(Qt::AlignCenter);
    m_hardwarePageLayout -> addWidget(m_hardwareTabWidget);

    m_hardwarePageWidget = new QWidget(this);
    m_hardwarePageWidget -> setLayout(m_hardwarePageLayout);

    qDebug() << "Hardware page created";
}

void MachineConfigWindow::createBootPage() {

    m_bootMenuCheckBox = new QCheckBox();
    m_bootMenuCheckBox -> setText(tr("Enable boot menu"));

    m_moveUpToolButton = new QToolButton();
    m_moveUpToolButton -> setArrowType(Qt::UpArrow);

    m_moveDownToolButton = new QToolButton();
    m_moveDownToolButton -> setArrowType(Qt::DownArrow);

    m_floppyTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_floppyTreeItem -> setIcon(0, QIcon::fromTheme("media-floppy",
                                                 QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_floppyTreeItem -> setText(0, tr("Floppy"));
    m_floppyTreeItem -> setData(0, Qt::UserRole, "a");
    m_floppyTreeItem -> setCheckState(0, Qt::Unchecked);

    m_CDROMTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_CDROMTreeItem -> setIcon(0, QIcon::fromTheme("media-optical",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_CDROMTreeItem -> setText(0, tr("CDROM"));
    m_CDROMTreeItem -> setData(0, Qt::UserRole, "d");
    m_CDROMTreeItem -> setCheckState(0, Qt::Unchecked);

    m_hardDiskTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_hardDiskTreeItem -> setIcon(0, QIcon::fromTheme("drive-harddisk",
                                                      QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_hardDiskTreeItem -> setText(0, tr("Hard Disk"));
    m_hardDiskTreeItem -> setData(0, Qt::UserRole, "c");
    m_hardDiskTreeItem -> setCheckState(0, Qt::Unchecked);

    m_networkTreeItem = new QTreeWidgetItem(QTreeWidgetItem::Type);
    m_networkTreeItem -> setIcon(0, QIcon::fromTheme("network-wired",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/dialog-cancel.svg"))));
    m_networkTreeItem -> setText(0, tr("Network"));
    m_networkTreeItem -> setData(0, Qt::UserRole, "n");
    m_networkTreeItem -> setCheckState(0, Qt::Unchecked);

    m_bootTree = new QTreeWidget();
    m_bootTree -> setMaximumHeight(120);
    m_bootTree -> setMaximumWidth(150);
    m_bootTree -> setColumnCount(1);
    m_bootTree -> setHeaderHidden(true);
    m_bootTree -> setRootIsDecorated(false);
    m_bootTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_bootTree -> insertTopLevelItem(0, m_floppyTreeItem);
    m_bootTree -> insertTopLevelItem(1, m_CDROMTreeItem);
    m_bootTree -> insertTopLevelItem(2, m_hardDiskTreeItem);
    m_bootTree -> insertTopLevelItem(3, m_networkTreeItem);

    m_kernelBootCheckBox = new QCheckBox();
    m_kernelBootCheckBox -> setText(tr("Enable direct kernel boot"));

    m_kernelPathLabel = new QLabel(tr("Kernel path") + ":");
    m_initrdLabel = new QLabel(tr("Initrd path") + ":");
    m_kernelArgsLabel = new QLabel(tr("Kernel args") + ":");

    m_kernelPathLineEdit = new QLineEdit();
    m_kernelPathLineEdit -> setPlaceholderText("/boot/vmlinuz-linux");
    m_initredLineEdit = new QLineEdit();
    m_initredLineEdit -> setPlaceholderText("/boot/initramfs-linux.img");
    m_kernelArgsLineEdit = new QLineEdit();

    m_kernelPathPushButton = new QPushButton();
    m_kernelPathPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                       QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    m_initrdPushButton = new QPushButton();
    m_initrdPushButton -> setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));

    m_bootTreeLayout = new QHBoxLayout();
    m_bootTreeLayout -> setAlignment(Qt::AlignTop);
    m_bootTreeLayout -> setSpacing(5);
    m_bootTreeLayout -> addWidget(m_bootTree);
    m_bootTreeLayout -> addWidget(m_moveUpToolButton);
    m_bootTreeLayout -> addWidget(m_moveDownToolButton);

    m_kernelLayout = new QGridLayout();
    m_kernelLayout -> setSpacing(5);
    m_kernelLayout -> addWidget(m_kernelPathLabel,      0, 0, 1, 1);
    m_kernelLayout -> addWidget(m_kernelPathLineEdit,   0, 1, 1, 1);
    m_kernelLayout -> addWidget(m_kernelPathPushButton, 0, 2, 1, 1);
    m_kernelLayout -> addWidget(m_initrdLabel,          1, 0, 1, 1);
    m_kernelLayout -> addWidget(m_initredLineEdit,      1, 1, 1, 1);
    m_kernelLayout -> addWidget(m_initrdPushButton,     1, 2, 1, 1);
    m_kernelLayout -> addWidget(m_kernelArgsLabel,      2, 0, 1, 1);
    m_kernelLayout -> addWidget(m_kernelArgsLineEdit,   2, 1, 1, 1);

    m_bootPageLayout = new QVBoxLayout();
    m_bootPageLayout -> setAlignment(Qt::AlignTop);
    m_bootPageLayout -> addWidget(m_bootMenuCheckBox);
    m_bootPageLayout -> addItem(m_bootTreeLayout);
    m_bootPageLayout -> addWidget(m_kernelBootCheckBox);
    m_bootPageLayout -> addItem(m_kernelLayout);

    m_bootPageWidget = new QWidget(this);
    m_bootPageWidget -> setLayout(m_bootPageLayout);

    qDebug() << "Boot page created";
}

void MachineConfigWindow::createMediaPage() {

    m_mediaItem = new QTreeWidgetItem();

    m_mediaTree = new QTreeWidget();
    m_mediaTree -> setMaximumHeight(250);
    m_mediaTree -> setMaximumWidth(200);
    m_mediaTree -> setColumnCount(1);
    m_mediaTree -> setHeaderHidden(true);
    m_mediaTree -> setRootIsDecorated(false);
    m_mediaTree -> setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_mediaPathLabel = new QLabel();
    m_mediaSizeLabel = new QLabel();
    m_mediaFormatLabel = new QLabel();

    m_mediaDetailsLayout = new QFormLayout();
    m_mediaDetailsLayout -> setAlignment(Qt::AlignTop);
    m_mediaDetailsLayout -> setLabelAlignment(Qt::AlignLeft);
    m_mediaDetailsLayout -> setHorizontalSpacing(20);
    m_mediaDetailsLayout -> setVerticalSpacing(10);
    m_mediaDetailsLayout -> addRow(tr("Path") + ":", m_mediaPathLabel);
    m_mediaDetailsLayout -> addRow(tr("Size") + ":", m_mediaSizeLabel);
    m_mediaDetailsLayout -> addRow(tr("Format") + ":", m_mediaFormatLabel);

    m_mediaSettingsGroupBox = new QGroupBox(tr("Details"));
    m_mediaSettingsGroupBox -> setLayout(m_mediaDetailsLayout);

    m_cacheComboBox = new QComboBox();
    m_cacheComboBox -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_cacheComboBox -> addItem("none");
    m_cacheComboBox -> addItem("writethrough");
    m_cacheComboBox -> addItem("writeback");
    m_cacheComboBox -> addItem("directsync");
    m_cacheComboBox -> addItem("unsafe");
    m_cacheComboBox -> setCurrentIndex(0);

    m_IOComboBox = new QComboBox();
    m_IOComboBox -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_IOComboBox -> addItem("threads");
    m_IOComboBox -> addItem("native");
    m_IOComboBox -> setCurrentIndex(0);

    m_readOnlyMediaCheck = new QCheckBox();

    m_mediaOptionsLayout = new QFormLayout();
    m_mediaOptionsLayout -> setAlignment(Qt::AlignTop);
    m_mediaOptionsLayout -> setLabelAlignment(Qt::AlignLeft);
    m_mediaOptionsLayout -> setHorizontalSpacing(20);
    m_mediaOptionsLayout -> setVerticalSpacing(10);
    m_mediaOptionsLayout -> addRow(tr("Cache mode") + ":", m_cacheComboBox);
    m_mediaOptionsLayout -> addRow(tr("IO mode") + ":", m_IOComboBox);
    m_mediaOptionsLayout -> addRow(tr("Read only") + ":", m_readOnlyMediaCheck);

    m_mediaOptionsGroupBox = new QGroupBox(tr("Options"));
    m_mediaOptionsGroupBox -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mediaOptionsGroupBox -> setLayout(m_mediaOptionsLayout);

    m_addHDDPushButton = new QPushButton();
    m_addHDDPushButton -> setIcon(QIcon::fromTheme("drive-harddisk",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));
    m_addHDDPushButton -> setToolTip(tr("Add hard disk media"));

    m_addCDROMPushButton = new QPushButton();
    m_addCDROMPushButton -> setIcon(QIcon::fromTheme("media-optical-data",
                                                     QIcon(QPixmap(":/images/icons/breeze/32x32/preferences-other.svg"))));
    m_addCDROMPushButton -> setToolTip(tr("Add optical media"));

    m_mediaAddLayout = new QHBoxLayout();
    m_mediaAddLayout -> setAlignment(Qt::AlignTop);
    m_mediaAddLayout -> addWidget(m_addHDDPushButton);
    m_mediaAddLayout -> addWidget(m_addCDROMPushButton);

    m_mediaAddGroupBox = new QGroupBox();
    m_mediaAddGroupBox -> setLayout(m_mediaAddLayout);
    m_mediaAddGroupBox -> setFlat(true);

    m_mediaPageLayout = new QGridLayout();
    m_mediaPageLayout -> setAlignment(Qt::AlignTop);
    m_mediaPageLayout -> setSpacing(1);
    m_mediaPageLayout -> addWidget(m_mediaTree,             0, 0, 1, 1);
    m_mediaPageLayout -> addWidget(m_mediaSettingsGroupBox, 0, 1, 1, 1);
    m_mediaPageLayout -> addWidget(m_mediaAddGroupBox,      1, 0, 1, 1);
    m_mediaPageLayout -> addWidget(m_mediaOptionsGroupBox,  1, 1, 1, 1);

    m_mediaPageWidget = new QWidget();
    m_mediaPageWidget -> setLayout(m_mediaPageLayout);

    qDebug() << "Media page created";
}

void MachineConfigWindow::saveMachineSettings() {

    qDebug() << "Machine settings saved";
}
