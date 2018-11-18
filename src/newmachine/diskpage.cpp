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
#include "diskpage.h"

MachineDiskPage::MachineDiskPage(Machine *machine,
                                 QWidget *parent) : QWizardPage(parent) {

    setTitle(tr("Machine virtual hard disk"));

    this->m_newMachine = machine;

    m_machineDiskLabel = new QLabel(tr("Select a virtual hard disk to the new machine."
                                       "You can either create a new hard disk or select an existing one."
                                       "Or leave empty"));
    m_machineDiskLabel -> setWordWrap(true);

    m_machineDiskInfoLabel = new QLabel(tr("If you need a more complex storage set-up"
                                           "you can skip this step and make the changes"
                                           "to the machine settings once the machine is created."));
    m_machineDiskInfoLabel -> setWordWrap(true);

    m_machineDiskSizeLabel = new QLabel(tr("The recommended size of the hard disk is"));
    m_machineDiskSizeLabel -> setWordWrap(true);

    m_noDiskRadio = new QRadioButton(tr("Do not add a virtual hard disk"));

    m_createDiskRadio = new QRadioButton(tr("Create a new virtual hard disk"));
    m_createDiskRadio -> setChecked(true);

    m_useExistingDiskRadio = new QRadioButton(tr("Use an existing virtual hard disk"));
    connect(m_useExistingDiskRadio, &QAbstractButton::toggled,
            this, &MachineDiskPage::useExistingDiskToggle);

    m_hardDiskPathLineEdit = new QLineEdit();
    m_hardDiskPathLineEdit -> setEnabled(false);

    m_pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                              "",
                                              this);

    connect(m_pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &MachineDiskPage::useExistingDiskPath);

    this->useExistingDiskToggle(false);

    this->registerField("noDisk", this->m_noDiskRadio);
    this->registerField("createDisk", this->m_createDiskRadio);
    this->registerField("useDisk", this->m_useExistingDiskRadio);
    this->registerField("machine.diskPath", this->m_hardDiskPathLineEdit);

    m_useOldDiskLayout = new QHBoxLayout();
    m_useOldDiskLayout -> setAlignment(Qt::AlignVCenter);
    m_useOldDiskLayout -> addWidget(m_hardDiskPathLineEdit);
    m_useOldDiskLayout -> addWidget(m_pathNewDiskPushButton);

    m_machineDiskLayout = new QVBoxLayout();
    m_machineDiskLayout -> setSpacing(5);
    m_machineDiskLayout -> addWidget(m_machineDiskLabel);
    m_machineDiskLayout -> addWidget(m_machineDiskInfoLabel);
    m_machineDiskLayout -> addWidget(m_machineDiskSizeLabel);
    m_machineDiskLayout -> addWidget(m_noDiskRadio);
    m_machineDiskLayout -> addWidget(m_createDiskRadio);
    m_machineDiskLayout -> addWidget(m_useExistingDiskRadio);
    m_machineDiskLayout -> addItem(m_useOldDiskLayout);

    setLayout(m_machineDiskLayout);

    qDebug() << "MachineDiskPage created";
}

MachineDiskPage::~MachineDiskPage() {
    qDebug() << "MachineDiskPage destroyed";
}

void MachineDiskPage::useExistingDiskToggle(bool toggled) {

    this->m_pathNewDiskPushButton->setEnabled(toggled);

    if( ! toggled ) {
        this->m_hardDiskPathLineEdit->clear();
    }
}

void MachineDiskPage::useExistingDiskPath() {

    this->m_existingDiskPath = QFileDialog::getSaveFileName(this,
                                                            tr("Select an existing disk"),
                                                            QDir::homePath(),
                                                            tr("All Files (*);;Images Files (*.img *.qcow *.qcow2 *.vmdk)"));

    if ( !m_existingDiskPath.isEmpty() ) {
        this -> m_hardDiskPathLineEdit -> setText(QDir::toNativeSeparators(m_existingDiskPath));
    }
}

int MachineDiskPage::nextId() const {
    if(this -> m_noDiskRadio -> isChecked() || this -> m_useExistingDiskRadio -> isChecked()) {
        return MachineWizard::Page_Conclusion;
    } else {
        return MachineWizard::Page_New_Disk;
    }
}

bool MachineDiskPage::validatePage() {
    if(this->m_useExistingDiskRadio->isChecked() && this->m_hardDiskPathLineEdit->text().isEmpty()) {
        m_notDiskMessageBox = new QMessageBox(this);
        m_notDiskMessageBox->setWindowTitle(tr("Qtemu - Disk not selected"));
        m_notDiskMessageBox->setIcon(QMessageBox::Warning);
        m_notDiskMessageBox->setText(tr("<p>Select a disk or another option</p>"));
        m_notDiskMessageBox->exec();

        return false;
    } else {
        return true;
    }
}

MachineNewDiskPage::MachineNewDiskPage(Machine *machine,
                                       QWidget *parent) : QWizardPage(parent) {

    this -> m_newMachine = machine;

    ////////////////////////////   DISK NAME   ///////////////////////////////////
    m_fileLocationGroupBox = new QGroupBox(tr("Disk name"));

    m_fileNameLineEdit = new QLineEdit();
    m_fileNameLineEdit->setEnabled(false);
    this->registerField("machine.diskname*", m_fileNameLineEdit);

    m_diskFormatLineEdit = new QLineEdit(this);
    m_diskFormatLineEdit->setEnabled(false);
    m_diskFormatLineEdit->setHidden(true);
    this->registerField("machine.diskFormat", m_diskFormatLineEdit);

    m_pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                              "",
                                              this);

    connect(m_pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &MachineNewDiskPage::selectNameNewDisk);

    m_fileLocationLayout = new QHBoxLayout();
    m_fileLocationLayout -> addWidget(m_fileNameLineEdit);
    m_fileLocationLayout -> addWidget(m_pathNewDiskPushButton);

    m_fileLocationGroupBox -> setLayout(m_fileLocationLayout);

    //////////////////////////   END DISK NAME   /////////////////////////////////

    ////////////////////////////   DISK SIZE   ///////////////////////////////////
    m_fileSizeGroupBox = new QGroupBox(tr("Disk size"));

    m_memoryLabel = new QLabel("GiB");
    m_diskSpinBox = new QDoubleSpinBox();
    m_diskSpinBox -> setMinimum(1);
    m_diskSpinBox -> setMaximum(1000000);
    m_diskSpinBox -> stepBy(1);


    m_diskSlider = new QSlider(Qt::Horizontal);
    m_diskSlider -> setTickPosition(QSlider::TicksBelow);
    m_diskSlider -> setTickInterval(10);
    m_diskSlider -> setMinimum(1);
    m_diskSlider -> setMaximum(100);

    connect(m_diskSlider, &QSlider::valueChanged,
            m_diskSpinBox, &QDoubleSpinBox::setValue);

    this->registerField("machine.diskSize", m_diskSpinBox, "value", "valueChanged");

    connect(m_diskSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            m_diskSlider, &QSlider::setValue);

    m_minDiskLabel = new QLabel("1 GiB");
    m_maxDisklabel = new QLabel("100 GiB");

    m_fileSizeLayout = new QGridLayout();
    m_fileSizeLayout -> setColumnStretch(1, 50);

    m_fileSizeLayout -> addWidget(m_diskSlider,   1, 0, 1, 3);
    m_fileSizeLayout -> addWidget(m_diskSpinBox,  1, 3, 1, 1);
    m_fileSizeLayout -> addWidget(m_memoryLabel,  1, 4, 1, 1);
    m_fileSizeLayout -> addWidget(m_minDiskLabel, 2, 0, 1, 1);
    m_fileSizeLayout -> addWidget(m_maxDisklabel, 2, 2, 1, 1);

    m_fileSizeGroupBox -> setLayout(m_fileSizeLayout);

    //////////////////////////   END DISK SIZE   /////////////////////////////////

    ////////////////////////////   DISK TYPE   ///////////////////////////////////
    m_fileTypeGroupBox = new QGroupBox(tr("Disk type"));

    m_rawRadioButton   = new QRadioButton(tr("raw"));
    m_qcowRadioButton  = new QRadioButton(tr("qcow (QEMU Copy-on-write)"));
    m_qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"));
    m_qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    m_vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    m_cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"));

    // Select by default qcow2 format
    m_qcow2RadioButton->setChecked(true);
    this->selectQCow2Format(true);

    connect(m_rawRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectRawFormat);
    connect(m_qcowRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQCowFormat);
    connect(m_qcow2RadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQCow2Format);
    connect(m_qedRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectQedFormat);
    connect(m_vmdkRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectVmdkFormat);
    connect(m_cloopRadioButton, &QAbstractButton::toggled,
                this, &MachineNewDiskPage::selectCloopFormat);

    m_diskTypeLayout = new QGridLayout();

    m_diskTypeLayout -> addWidget(m_rawRadioButton,   0, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_qcowRadioButton,  0, 1, 1, 1);
    m_diskTypeLayout -> addWidget(m_qcow2RadioButton, 1, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_qedRadioButton,   1, 1, 1, 1);
    m_diskTypeLayout -> addWidget(m_vmdkRadioButton,  2, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_cloopRadioButton, 2, 1, 1, 1);

    m_fileTypeGroupBox -> setLayout(m_diskTypeLayout);

    //////////////////////////   END DISK TYPE   /////////////////////////////////

    m_newDiskLayout = new QVBoxLayout();
    m_newDiskLayout -> addWidget(m_fileLocationGroupBox);
    m_newDiskLayout -> addWidget(m_fileSizeGroupBox);
    m_newDiskLayout -> addWidget(m_fileTypeGroupBox);

    setLayout(m_newDiskLayout);

    qDebug() << "MachineNewDiskPage created";
}

MachineNewDiskPage::~MachineNewDiskPage() {
    qDebug() << "MachineNewDiskPage destroyed";
}

void MachineNewDiskPage::initializePage() {
    m_fileNameLineEdit -> setText(field("machine.name").toString());
    this->m_qcow2RadioButton->setChecked(true);
}

bool MachineNewDiskPage::validatePage() {
    return true;
}

void MachineNewDiskPage::selectRawFormat(bool useRaw) {
    if(useRaw) {
        this->m_diskFormatLineEdit->setText("raw");
    }
}

void MachineNewDiskPage::selectQCowFormat(bool useQCow) {
    if(useQCow) {
        this->m_diskFormatLineEdit->setText("qcow");
    }
}

void MachineNewDiskPage::selectQCow2Format(bool useQCow2) {
    if(useQCow2) {
        this->m_diskFormatLineEdit->setText("qcow2");
    }
}

void MachineNewDiskPage::selectQedFormat(bool useQed) {
    if(useQed) {
        this->m_diskFormatLineEdit->setText("qed");
    }
}

void MachineNewDiskPage::selectVmdkFormat(bool useVmdk) {
    if(useVmdk) {
        this->m_diskFormatLineEdit->setText("vmdk");
    }
}

void MachineNewDiskPage::selectCloopFormat(bool useCloop) {
    if(useCloop) {
        this->m_diskFormatLineEdit->setText("cloop");
    }
}

void MachineNewDiskPage::selectNameNewDisk() {

    this -> m_diskName = QFileDialog::getSaveFileName(this,
                                                      tr("Select an existing disk"),
                                                      QDir::homePath(),
                                                      tr("All Files (*);;Images Files (*.img *.qcow *.qcow2 *.vmdk)"));

    if ( ! this -> m_diskName.isEmpty() ) {
        this->m_fileNameLineEdit->setText(QDir::toNativeSeparators(m_diskName));
    }
}
