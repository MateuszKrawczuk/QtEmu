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
#include "diskpage.h"

MachineDiskPage::MachineDiskPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine virtual hard disk"));

    this -> newMachine = machine;
    this -> newMachine -> setCreateNewDisk(false);

    machineDiskLabel = new QLabel(tr("Select a virtual hard disk to the new machine."
                                     "You can either create a new hard disk or select an existing one."
                                     "Or leave empty"));
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

    connect(pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &MachineDiskPage::useExistingDiskPath);

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

    if( ! toggled ) {
        this -> hardDiskPathLineEdit -> clear();
        this -> newMachine -> setDiskPath("");
    }
}

void MachineDiskPage::useExistingDiskPath() {

    this -> existingDiskPath = QFileDialog::getSaveFileName(this,
                                                            tr("Select an existing disk"),
                                                            QDir::homePath(),
                                                            tr("All Files (*);;Images Files (*.img *.qcow *.qcow2 *.wmdk)"));

    if ( !existingDiskPath.isEmpty() ) {
        this -> hardDiskPathLineEdit -> setText(QDir::toNativeSeparators(existingDiskPath));
        this -> newMachine -> setDiskPath(QDir::toNativeSeparators(existingDiskPath));

        QFileInfo machineDiskName(QDir::toNativeSeparators(existingDiskPath));
        this -> newMachine -> setDiskName(machineDiskName.fileName());
    }
}

int MachineDiskPage::nextId() const {
    if(this -> noDiskRadio -> isChecked() || this -> useExistingDiskRadio -> isChecked()) {
        return MachineWizard::Page_Conclusion;
    } else {
        return MachineWizard::Page_New_Disk;
    }
}

bool MachineDiskPage::validatePage() {

    if(this -> noDiskRadio -> isChecked()) {
        this -> newMachine -> setDiskName("");
        this -> newMachine -> setDiskPath("");
        this -> newMachine -> setDiskSize(0);
        this -> newMachine -> setDiskFormat("");
    } else if(this -> useExistingDiskRadio -> isChecked()) {
        this -> newMachine -> setDiskSize(0);
        this -> newMachine -> setDiskFormat("");
    }

    if(this -> useExistingDiskRadio -> isChecked() && this -> hardDiskPathLineEdit -> text().isEmpty()) {
        notDiskMessageBox = new QMessageBox(this);
        notDiskMessageBox -> setWindowTitle(tr("Qtemu - Disk not selected"));
        notDiskMessageBox -> setIcon(QMessageBox::Warning);
        notDiskMessageBox -> setText(tr("<p>Select a disk or another option</p>"));
        notDiskMessageBox -> exec();

        return false;
    } else {
        return true;
    }
}

MachineNewDiskPage::MachineNewDiskPage(Machine *machine,
                                       QWidget *parent) : QWizardPage(parent) {

    this -> newMachine = machine;

    ////////////////////////////   DISK NAME   ///////////////////////////////////
    fileLocationGroupBox = new QGroupBox(tr("Disk name"));

    fileNameLineEdit = new QLineEdit();
    fileNameLineEdit -> setEnabled(false);

    this -> registerField("machine.diskname*", fileNameLineEdit);

    pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                             QIcon(":/icon/32x32/qtemu.png")),
                                            "",
                                            this);

    connect(pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &MachineNewDiskPage::selectNameNewDisk);

    fileLocationLayout = new QHBoxLayout();
    fileLocationLayout -> addWidget(fileNameLineEdit);
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
    this -> selectQCow2Format(true);
    qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"));

    connect(rawRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectRawFormat);
    connect(qcowRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQCowFormat);
    connect(qcow2RadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQCow2Format);
    connect(qedRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQedFormat);
    connect(vmdkRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectVmdkFormat);
    connect(cloopRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectCloopFormat);

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
    fileNameLineEdit -> setText(field("machine.name").toString());
    this -> newMachine -> setDiskName(field("machine.name").toString());
    this -> newMachine -> setDiskPath("");

    this -> qcow2RadioButton -> setChecked(true);
}

bool MachineNewDiskPage::validatePage() {

    this -> newMachine -> setCreateNewDisk(true);
    this -> newMachine -> setDiskFormat(this -> diskFormat);
    this -> newMachine -> setDiskSize(this -> diskSpinBox -> value());

    return true;
}

void MachineNewDiskPage::selectRawFormat(bool useRaw) {
    if(useRaw) {
        this -> diskFormat = "raw";
    }
}

void MachineNewDiskPage::selectQCowFormat(bool useQCow) {
    if(useQCow) {
        this -> diskFormat = "qcow";
    }
}

void MachineNewDiskPage::selectQCow2Format(bool useQCow2) {
    if(useQCow2) {
        this -> diskFormat = "qcow2";
    }
}

void MachineNewDiskPage::selectQedFormat(bool useQed) {
    if(useQed) {
        this -> diskFormat = "qed";
    }
}

void MachineNewDiskPage::selectVmdkFormat(bool useVmdk) {
    if(useVmdk) {
        this -> diskFormat = "vmdk";
    }
}

void MachineNewDiskPage::selectCloopFormat(bool useCloop) {
    if(useCloop) {
        this -> diskFormat = "cloop";
    }
}

void MachineNewDiskPage::selectNameNewDisk() {

    this -> diskName = QFileDialog::getSaveFileName(this,
                                                    tr("Select an existing disk"),
                                                    QDir::homePath(),
                                                    tr("All Files (*);;Images Files (*.img *.qcow *.qcow2 *.wmdk)"));

    if ( ! this -> diskName.isEmpty() ) {
        this -> fileNameLineEdit -> setText(QDir::toNativeSeparators(diskName));
        this -> newMachine -> setDiskPath(QDir::toNativeSeparators(diskName));

        QFileInfo machineDiskName(QDir::toNativeSeparators(diskName));
        this -> newMachine -> setDiskName(machineDiskName.fileName());
    }

}
