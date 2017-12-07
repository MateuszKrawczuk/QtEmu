/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "diskpage.h"

MachineDiskPage::MachineDiskPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine virtual hard disk"));

    this -> newMachine = machine;

    machineDiskLabel = new QLabel(
                tr("Select a virtual hard disk to the new machine."
                   "You can either create a new hard disk or select an existing one."));
    machineDiskLabel -> setWordWrap(true);

    machineDiskInfoLabel = new QLabel(tr("If you need a more complex storage set-up"
                                         "you can skip this step and make the changes"
                                         "to the machine settings once the machine is created."));
    machineDiskInfoLabel -> setWordWrap(true);

    machineDiskSizeLabel = new QLabel(tr("The recommended size of the hard disk is"));
    machineDiskSizeLabel -> setWordWrap(true);

    noDiskRadio = new QRadioButton(tr("Do not add a virtual hard disk"));

    createDiskRadio = new QRadioButton(tr("Create a new virtual hard disk"));
    createDiskRadio -> setChecked(true);

    useExistingDiskRadio = new QRadioButton(tr("Use an existing virtual hard disk"));

    connect(useExistingDiskRadio, &QAbstractButton::toggled,
            this, &MachineDiskPage::useExistingDiskToggle);

    hardDiskPathLineEdit = new QLineEdit();
    hardDiskPathLineEdit -> setEnabled(false);

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    this -> useExistingDiskToggle(false);

    useOldDiskLayout = new QHBoxLayout();
    useOldDiskLayout -> setAlignment(Qt::AlignVCenter);
    useOldDiskLayout -> addWidget(hardDiskPathLineEdit);
    useOldDiskLayout -> addWidget(pathNewDiskPushButton);

    machineDiskLayout = new QVBoxLayout();
    machineDiskLayout -> setSpacing(5);
    machineDiskLayout -> addWidget(machineDiskLabel);
    machineDiskLayout -> addWidget(machineDiskInfoLabel);
    machineDiskLayout -> addWidget(machineDiskSizeLabel);
    machineDiskLayout -> addWidget(noDiskRadio);
    machineDiskLayout -> addWidget(createDiskRadio);
    machineDiskLayout -> addWidget(useExistingDiskRadio);
    machineDiskLayout -> addItem(useOldDiskLayout);

    setLayout(machineDiskLayout);

    qDebug() << "MachineDiskPage created";
}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

void MachineDiskPage::useExistingDiskToggle(bool toggled) {

    this -> pathNewDiskPushButton -> setEnabled(toggled);
}

MachineNewDiskPage::MachineNewDiskPage(Machine *machine,
                                       QWidget *parent) : QWizardPage(parent) {

    this -> newMachine = machine;

    ////////////////////////////   DISK NAME   ///////////////////////////////////
    fileLocationGroupBox = new QGroupBox(tr("Disk name"));

    fileName = new QLineEdit();

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                        "",
                                        this);

    fileLocationLayout = new QHBoxLayout();
    fileLocationLayout -> addWidget(fileName);
    fileLocationLayout -> addWidget(pathNewDiskPushButton);

    fileLocationGroupBox -> setLayout(fileLocationLayout);

    //////////////////////////   END DISK NAME   /////////////////////////////////

    ////////////////////////////   DISK SIZE   ///////////////////////////////////
    fileSizeGroupBox = new QGroupBox(tr("Disk size"));

    memoryLabel = new QLabel("GiB");
    diskSpinBox = new QDoubleSpinBox();
    diskSpinBox -> setMinimum(1);
    diskSpinBox -> setMaximum(1000000);
    diskSpinBox -> stepBy(1);

    diskSlider = new QSlider(Qt::Horizontal);
    diskSlider -> setTickPosition(QSlider::TicksBelow);
    diskSlider -> setTickInterval(10);
    diskSlider -> setMinimum(1);
    diskSlider -> setMaximum(100);

    connect(diskSlider, &QSlider::valueChanged,
            diskSpinBox, &QDoubleSpinBox::setValue);

    connect(diskSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            diskSlider, &QSlider::setValue);

    minDiskLabel = new QLabel("1 GiB");
    maxDisklabel = new QLabel("100 GiB");

    fileSizeLayout = new QGridLayout();
    fileSizeLayout -> setColumnStretch(1, 50);

    fileSizeLayout -> addWidget(diskSlider,   1, 0, 1, 3);
    fileSizeLayout -> addWidget(diskSpinBox,  1, 3, 1, 1);
    fileSizeLayout -> addWidget(memoryLabel,  1, 4, 1, 1);
    fileSizeLayout -> addWidget(minDiskLabel, 2, 0, 1, 1);
    fileSizeLayout -> addWidget(maxDisklabel, 2, 2, 1, 1);

    fileSizeGroupBox -> setLayout(fileSizeLayout);

    //////////////////////////   END DISK SIZE   /////////////////////////////////

    ////////////////////////////   DISK TYPE   ///////////////////////////////////
    fileTypeGroupBox = new QGroupBox(tr("Disk type"));

    rawRadioButton   = new QRadioButton(tr("raw"));
    qcowRadioButton  = new QRadioButton(tr("qcow (QEMU Copy-on-write)"));
    qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"));
    qcow2RadioButton -> setChecked(true);
    qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"));

    diskTypeLayout = new QGridLayout();

    diskTypeLayout -> addWidget(rawRadioButton,  0, 0, 1, 1);
    diskTypeLayout -> addWidget(qcowRadioButton,  0, 1, 1, 1);
    diskTypeLayout -> addWidget(qcow2RadioButton, 1, 0, 1, 1);
    diskTypeLayout -> addWidget(qedRadioButton,   1, 1, 1, 1);
    diskTypeLayout -> addWidget(vmdkRadioButton,  2, 0, 1, 1);
    diskTypeLayout -> addWidget(cloopRadioButton, 2, 1, 1, 1);

    fileTypeGroupBox -> setLayout(diskTypeLayout);

    //////////////////////////   END DISK TYPE   /////////////////////////////////

    newDiskLayout = new QVBoxLayout();
    newDiskLayout -> addWidget(fileLocationGroupBox);
    newDiskLayout -> addWidget(fileSizeGroupBox);
    newDiskLayout -> addWidget(fileTypeGroupBox);

    setLayout(newDiskLayout);

    qDebug() << "MachineNewDiskPage created";
}

MachineNewDiskPage::~MachineNewDiskPage() {
    qDebug() << "MachineNewDiskPage destroyed";
}

void MachineNewDiskPage::initializePage() {
    fileName -> setText(field("machine.name").toString());
}
