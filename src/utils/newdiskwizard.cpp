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
#include "newdiskwizard.h"

NewDiskWizard::NewDiskWizard(Machine *machine,
                             QEMU *QEMUGlobalObject,
                             QWidget *parent) : QWizard(parent) {

    setWindowTitle(tr("Create a new Disk"));

    setPage(Page_Disk, new NewDiskPage(machine, QEMUGlobalObject, this));

    setStartId(Page_Disk);

    #ifndef Q_OS_MAC
        setWizardStyle(ClassicStyle);
    #endif

    #ifdef Q_OS_MAC
        setWizardStyle(MacStyle);
    #endif

    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));

    setMinimumSize(700, 400);

    qDebug() << "NewDiskWizard created";
}

NewDiskWizard::~NewDiskWizard() {
    qDebug() << "NewDiskWizard destroyed";
}

NewDiskPage::NewDiskPage(Machine *machine,
                         QEMU *QEMUGlobalObject,
                         QWidget *parent) : QWizardPage(parent) {

    this -> m_newMachine = machine;
    this -> m_qemuGlobalObject = QEMUGlobalObject;

    m_fileLocationGroupBox = new QGroupBox(tr("Disk path"));

    QString newDiskPath = this -> m_newMachine -> getName().append("NewDisk");

    this -> m_diskName = this -> m_newMachine -> getPath()
                         .append(QDir::toNativeSeparators("/"))
                         .append(this -> m_newMachine -> getName())
                         .append("NewDisk")
                         .append(".qcow2");

    m_fileNameLineEdit = new QLineEdit();
    m_fileNameLineEdit -> setEnabled(false);
    m_fileNameLineEdit -> setText(newDiskPath);

    m_pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                              "",
                                              this);

    connect(m_pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &NewDiskPage::selectNameNewDisk);

    m_fileLocationLayout = new QHBoxLayout();
    m_fileLocationLayout -> addWidget(m_fileNameLineEdit);
    m_fileLocationLayout -> addWidget(m_pathNewDiskPushButton);

    m_fileLocationGroupBox -> setLayout(m_fileLocationLayout);

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

    m_fileTypeGroupBox = new QGroupBox(tr("Disk type"));

    m_rawRadioButton   = new QRadioButton(tr("raw"));
    m_qcowRadioButton  = new QRadioButton(tr("qcow (QEMU Copy-on-write)"));
    m_qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"));
    m_qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"));
    m_vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"));
    m_cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"));

    m_qcow2RadioButton -> setChecked(true);

    m_diskTypeLayout = new QGridLayout();

    m_diskTypeLayout -> addWidget(m_rawRadioButton,   0, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_qcowRadioButton,  0, 1, 1, 1);
    m_diskTypeLayout -> addWidget(m_qcow2RadioButton, 1, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_qedRadioButton,   1, 1, 1, 1);
    m_diskTypeLayout -> addWidget(m_vmdkRadioButton,  2, 0, 1, 1);
    m_diskTypeLayout -> addWidget(m_cloopRadioButton, 2, 1, 1, 1);

    m_fileTypeGroupBox -> setLayout(m_diskTypeLayout);

    m_newDiskLayout = new QVBoxLayout();
    m_newDiskLayout -> addWidget(m_fileLocationGroupBox);
    m_newDiskLayout -> addWidget(m_fileSizeGroupBox);
    m_newDiskLayout -> addWidget(m_fileTypeGroupBox);

    setLayout(m_newDiskLayout);

    qDebug() << "NewDiskPage created";
}

NewDiskPage::~NewDiskPage() {
    qDebug() << "NewDiskPage destroyed";
}

void NewDiskPage::selectNameNewDisk() {

    QString newDiskPath = this -> m_newMachine -> getPath()
                          .append(QDir::toNativeSeparators("/"))
                          .append(this -> m_newMachine -> getName())
                          .append("NewDisk")
                          .append(".")
                          .append(NewDiskPage::getExtension());

    this -> m_diskName = QFileDialog::getSaveFileName(this,
                                                      tr("Select a location for the new hard disk"),
                                                      newDiskPath,
                                                      tr("Image Files (*.raw *.qcow *.qcow2 *.qed *.vmdk *.cloop)"));

    if ( ! this -> m_diskName.isEmpty() ) {
        this -> m_fileNameLineEdit -> setText(QDir::toNativeSeparators(m_diskName));
    }
}

QString NewDiskPage::getExtension() {

    QString extension = "qcow2";

    if (this -> m_rawRadioButton -> isChecked()) {
        extension = "raw";
    } else if (this -> m_qcowRadioButton -> isChecked()) {
        extension = "qcow";
    } else if (this -> m_qcow2RadioButton -> isChecked()) {
        extension = "qcow2";
    } else if (this -> m_qedRadioButton -> isChecked()) {
        extension = "qed";
    } else if (this -> m_vmdkRadioButton -> isChecked()) {
        extension = "vmdk";
    } else if (this -> m_cloopRadioButton -> isChecked()) {
        extension = "cloop";
    }

    return extension;
}

bool NewDiskPage::validatePage() {

    bool isDiskCreated = SystemUtils::createDisk(this -> m_qemuGlobalObject,
                                                 this -> m_diskName,
                                                 NewDiskPage::getExtension(),
                                                 this -> m_diskSpinBox -> value(),
                                                 false);

    if (isDiskCreated) {
        Media newDisk;

/*        newDisk.setMediaName();
        newDisk.setMediaPath();
        newDisk.setMediaSize();
        newDisk.setMediaType();
        newDisk.setMediaFormat();
        newDisk.setMediaInterface();
        newDisk.setMediaCache();
        newDisk.setMediaIO();
        newDisk.setUuid();*/
    }

    return isDiskCreated;
}
