// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-FileCopyrightText: 2022 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "newdiskwizard.h"

/**
 * @brief Wizard to create a new disk
 * @param machine, machine to be configured
 * @param QEMUGlobalObject, QEMU global object with data about QEMU
 * @param media, new media
 * @param parent, parent widget
 *
 * Wizard to create a new disk
 */
NewDiskWizard::NewDiskWizard(Machine *machine,
                             QEMU *QEMUGlobalObject,
                             Media *media,
                             QWidget *parent) : QWizard(parent)
{
    this->setWindowTitle(tr("Create a new Disk"));
    this->setPage(Page_Disk, new NewDiskPage(machine, QEMUGlobalObject, media, this));
    this->setStartId(Page_Disk);

#ifndef Q_OS_MAC
    this->setWizardStyle(ClassicStyle);
#endif
#ifdef Q_OS_MAC
    this->setWizardStyle(MacStyle);
#endif

    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banner.png"));
    this->setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/banner.png"));
    this->setMinimumSize(700, 400);

    qDebug() << "NewDiskWizard created";
}

NewDiskWizard::~NewDiskWizard()
{
    qDebug() << "NewDiskWizard destroyed";
}

/**
 * @brief New disk page
 * @param machine, machine to be configured
 * @param QEMUGlobalObject, QEMU global object with data about QEMU
 * @param media, new media
 * @param parent, parent widget
 *
 * New disk page
 */
NewDiskPage::NewDiskPage(Machine *machine,
                         QEMU *QEMUGlobalObject,
                         Media *media,
                         QWidget *parent) : QWizardPage(parent)
{
    this->m_machineConfig = machine;
    this->m_newMedia = media;
    this->m_qemuGlobalObject = QEMUGlobalObject;

    m_fileLocationGroupBox = new QGroupBox(tr("Disk path"), this);

    QString newDiskPath = this->m_machineConfig->getName().append("NewDisk");

    this->m_diskPath = this->m_machineConfig->getPath()
                                              .append(QDir::toNativeSeparators("/"))
                                              .append(this->m_machineConfig->getName())
                                              .append("NewDisk")
                                              .append(".qcow2");

    m_fileNameLineEdit = new QLineEdit(this);
    m_fileNameLineEdit->setEnabled(false);
    m_fileNameLineEdit->setText(newDiskPath);

    m_pathNewDiskPushButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                               QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                              "",
                                              this);

    connect(m_pathNewDiskPushButton, &QAbstractButton::clicked,
            this, &NewDiskPage::selectNameNewDisk);

    m_fileLocationLayout = new QHBoxLayout();
    m_fileLocationLayout->addWidget(m_fileNameLineEdit);
    m_fileLocationLayout->addWidget(m_pathNewDiskPushButton);

    m_fileLocationGroupBox->setLayout(m_fileLocationLayout);

    m_fileSizeGroupBox = new QGroupBox(tr("Disk size"), this);

    m_memoryLabel = new QLabel("GiB", this);
    m_diskSpinBox = new QDoubleSpinBox(this);
    m_diskSpinBox->setMinimum(1);
    m_diskSpinBox->setMaximum(1000000);
    m_diskSpinBox->stepBy(1);

    m_diskSlider = new QSlider(Qt::Horizontal, this);
    m_diskSlider->setTickPosition(QSlider::TicksBelow);
    m_diskSlider->setTickInterval(10);
    m_diskSlider->setMinimum(1);
    m_diskSlider->setMaximum(100);

    connect(m_diskSlider, &QSlider::valueChanged,
            m_diskSpinBox, &QDoubleSpinBox::setValue);

    connect(m_diskSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            m_diskSlider, &QSlider::setValue);

    m_minDiskLabel = new QLabel("1 GiB", this);
    m_maxDisklabel = new QLabel("100 GiB", this);

    m_fileSizeLayout = new QGridLayout();
    m_fileSizeLayout->setColumnStretch(1, 50);

    m_fileSizeLayout->addWidget(m_diskSlider,   1, 0, 1, 3);
    m_fileSizeLayout->addWidget(m_diskSpinBox,  1, 3, 1, 1);
    m_fileSizeLayout->addWidget(m_memoryLabel,  1, 4, 1, 1);
    m_fileSizeLayout->addWidget(m_minDiskLabel, 2, 0, 1, 1);
    m_fileSizeLayout->addWidget(m_maxDisklabel, 2, 2, 1, 1);

    m_fileSizeGroupBox->setLayout(m_fileSizeLayout);

    m_fileTypeGroupBox = new QGroupBox(tr("Disk type"), this);

    m_rawRadioButton   = new QRadioButton(tr("raw"), this);
    m_qcowRadioButton  = new QRadioButton(tr("qcow (QEMU Copy-on-write)"), this);
    m_qcow2RadioButton = new QRadioButton(tr("qcow2 (QEMU Copy-on-write 2)"), this);
    m_qedRadioButton   = new QRadioButton(tr("qed (QEMU enhanced disk)"), this);
    m_vmdkRadioButton  = new QRadioButton(tr("vmdk (Virtual Machine Disk)"), this);
    m_cloopRadioButton = new QRadioButton(tr("cloop (Compressed Loop)"), this);

    m_qcow2RadioButton->setChecked(true);

    m_diskTypeLayout = new QGridLayout();
    m_diskTypeLayout->addWidget(m_rawRadioButton,   0, 0, 1, 1);
    m_diskTypeLayout->addWidget(m_qcowRadioButton,  0, 1, 1, 1);
    m_diskTypeLayout->addWidget(m_qcow2RadioButton, 1, 0, 1, 1);
    m_diskTypeLayout->addWidget(m_qedRadioButton,   1, 1, 1, 1);
    m_diskTypeLayout->addWidget(m_vmdkRadioButton,  2, 0, 1, 1);
    m_diskTypeLayout->addWidget(m_cloopRadioButton, 2, 1, 1, 1);

    m_fileTypeGroupBox->setLayout(m_diskTypeLayout);

    m_newDiskLayout = new QVBoxLayout();
    m_newDiskLayout->addWidget(m_fileLocationGroupBox);
    m_newDiskLayout->addWidget(m_fileSizeGroupBox);
    m_newDiskLayout->addWidget(m_fileTypeGroupBox);

    this->setLayout(m_newDiskLayout);

    qDebug() << "NewDiskPage created";
}

NewDiskPage::~NewDiskPage()
{
    qDebug() << "NewDiskPage destroyed";
}

/**
 * @brief Select the
 */
void NewDiskPage::selectNameNewDisk()
{
    QString newDiskPath = this->m_machineConfig->getPath()
                                                 .append(QDir::toNativeSeparators("/"))
                                                 .append(this->m_machineConfig->getName())
                                                 .append("NewDisk")
                                                 .append(".")
                                                 .append(NewDiskPage::getExtension());

    this->m_diskPath = QFileDialog::getSaveFileName(this,
                                                    tr("Select a location for the new hard disk"),
                                                    newDiskPath,
                                                    tr("Image Files (*.raw *.qcow *.qcow2 *.qed *.vmdk *.cloop)"));

    if (!this->m_diskPath.isEmpty()) {
        this->m_fileNameLineEdit->setText(QDir::toNativeSeparators(this->m_diskPath));
    }
}

/**
 * @brief Get the selected extension
 * @return selected extension
 *
 * Get the selected extension
 */
QString NewDiskPage::getExtension()
{
    QString extension = "qcow2";

    if (this->m_rawRadioButton->isChecked()) {
        extension = "raw";
    } else if (this->m_qcowRadioButton->isChecked()) {
        extension = "qcow";
    } else if (this->m_qcow2RadioButton->isChecked()) {
        extension = "qcow2";
    } else if (this->m_qedRadioButton->isChecked()) {
        extension = "qed";
    } else if (this->m_vmdkRadioButton->isChecked()) {
        extension = "vmdk";
    } else if (this->m_cloopRadioButton->isChecked()) {
        extension = "cloop";
    }

    return extension;
}

/**
 * @brief Validate the page
 * @return true if the disk is created
 *
 * Validate the page and create the disk
 */
bool NewDiskPage::validatePage()
{
    bool isDiskCreated = SystemUtils::createDisk(this->m_qemuGlobalObject,
                                                 this->m_diskPath,
                                                 NewDiskPage::getExtension(),
                                                 this->m_diskSpinBox->value(),
                                                 false);

    QFileInfo newDiskInfo(this->m_diskPath);

    if (isDiskCreated) {
        this->m_newMedia->setName(newDiskInfo.fileName());
        this->m_newMedia->setPath(QDir::toNativeSeparators(newDiskInfo.absoluteFilePath()));
        this->m_newMedia->setType("hdd");
        this->m_newMedia->setUuid(QUuid::createUuid());
    }

    return isDiskCreated;
}
