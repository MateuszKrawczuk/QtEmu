// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "machineconfigbios.h"

/**
 * @brief BIOS configuration window
 * @param machine, machine to be configured
 * @param qemuObject, QEMU object with BIOS directory information
 * @param parent, parent widget
 *
 * In this window the user can configure the BIOS of the machine,
 * including both a specific BIOS file (-bios) and a BIOS search
 * directory (-L) for QEMU to find firmware files.
 */
MachineConfigBios::MachineConfigBios(Machine *machine,
                                     QEMU *qemuObject,
                                     QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;
    this->m_qemuObject = qemuObject;

    // --- BIOS search directory section (QEMU -L flag) ---

    m_useCustomBiosDirCheckBox = new QCheckBox(this);
    m_useCustomBiosDirCheckBox->setEnabled(enableFields);
    m_useCustomBiosDirCheckBox->setText(tr("Use custom BIOS search directory (-L)"));
    m_useCustomBiosDirCheckBox->setChecked(!this->m_machine->getBiosDirectory().isEmpty());
    connect(m_useCustomBiosDirCheckBox, &QAbstractButton::toggled,
            this, &MachineConfigBios::customBiosDirToggled);

    m_biosDirLabel = new QLabel(tr("BIOS directory") + ":", this);
    m_biosDirLabel->setEnabled(m_useCustomBiosDirCheckBox->isChecked());

    m_biosDirLineEdit = new QLineEdit(this);
    m_biosDirLineEdit->setEnabled(enableFields && m_useCustomBiosDirCheckBox->isChecked());
    m_biosDirLineEdit->setText(this->m_machine->getBiosDirectory());
    m_biosDirLineEdit->setPlaceholderText(this->m_qemuObject->BIOSDirectory());

    m_biosDirPushButton = new QPushButton(this);
    m_biosDirPushButton->setEnabled(enableFields && m_useCustomBiosDirCheckBox->isChecked());
    m_biosDirPushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_biosDirPushButton, &QAbstractButton::clicked,
            this, &MachineConfigBios::selectBiosDirectory);

    m_biosDirLayout = new QHBoxLayout();
    m_biosDirLayout->setSpacing(5);
    m_biosDirLayout->addWidget(m_biosDirLabel);
    m_biosDirLayout->addWidget(m_biosDirLineEdit);
    m_biosDirLayout->addWidget(m_biosDirPushButton);

    // --- Custom BIOS file section (QEMU -bios flag) ---

    m_useCustomBiosCheckBox = new QCheckBox(this);
    m_useCustomBiosCheckBox->setEnabled(enableFields);
    m_useCustomBiosCheckBox->setText(tr("Use custom BIOS file (-bios)"));
    m_useCustomBiosCheckBox->setChecked(!this->m_machine->getBiosPath().isEmpty());
    connect(m_useCustomBiosCheckBox, &QAbstractButton::toggled,
            this, &MachineConfigBios::customBiosToggled);

    m_biosFileLabel = new QLabel(tr("BIOS file") + ":", this);
    m_biosFileLabel->setEnabled(m_useCustomBiosCheckBox->isChecked());

    m_biosComboBox = new QComboBox(this);
    m_biosComboBox->setEnabled(enableFields && m_useCustomBiosCheckBox->isChecked());
    m_biosComboBox->addItem(tr("Default"));
    m_biosComboBox->addItems(this->m_qemuObject->availableBIOSFiles());

    // If machine already has a BIOS set, select it in the combobox
    if (!this->m_machine->getBiosPath().isEmpty()) {
        QString biosFileName = QFileInfo(this->m_machine->getBiosPath()).fileName();
        int index = m_biosComboBox->findText(biosFileName);
        if (index >= 0) {
            m_biosComboBox->setCurrentIndex(index);
        } else {
            m_biosComboBox->addItem(biosFileName);
            m_biosComboBox->setCurrentIndex(m_biosComboBox->count() - 1);
        }
    }

    m_biosFilePushButton = new QPushButton(this);
    m_biosFilePushButton->setEnabled(enableFields && m_useCustomBiosCheckBox->isChecked());
    m_biosFilePushButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    connect(m_biosFilePushButton, &QAbstractButton::clicked,
            this, &MachineConfigBios::selectBiosFile);

    m_biosFileLayout = new QHBoxLayout();
    m_biosFileLayout->setSpacing(5);
    m_biosFileLayout->addWidget(m_biosFileLabel);
    m_biosFileLayout->addWidget(m_biosComboBox);
    m_biosFileLayout->addWidget(m_biosFilePushButton);

    // --- Page layout ---

    m_biosPageLayout = new QVBoxLayout();
    m_biosPageLayout->setAlignment(Qt::AlignTop);
    m_biosPageLayout->addWidget(m_useCustomBiosDirCheckBox);
    m_biosPageLayout->addItem(m_biosDirLayout);
    m_biosPageLayout->addSpacing(10);
    m_biosPageLayout->addWidget(m_useCustomBiosCheckBox);
    m_biosPageLayout->addItem(m_biosFileLayout);

    m_biosPageWidget = new QWidget(this);
    m_biosPageWidget->setLayout(m_biosPageLayout);

    // Set initial state of controls
    this->customBiosDirToggled(m_useCustomBiosDirCheckBox->isChecked());
    this->customBiosToggled(m_useCustomBiosCheckBox->isChecked());

    qDebug() << "MachineConfigBios created";
}

MachineConfigBios::~MachineConfigBios()
{
    qDebug() << "MachineConfigBios destroyed";
}

/**
 * @brief Enable or disable BIOS file options
 * @param checked, true if custom BIOS file is enabled
 *
 * Enable or disable BIOS file options based on checkbox state
 */
void MachineConfigBios::customBiosToggled(bool checked)
{
    this->m_biosFileLabel->setEnabled(checked);
    this->m_biosComboBox->setEnabled(checked);
    this->m_biosFilePushButton->setEnabled(checked);

    if (!checked) {
        this->m_biosComboBox->setCurrentIndex(0);
    }
}

/**
 * @brief Enable or disable BIOS directory options
 * @param checked, true if custom BIOS directory is enabled
 *
 * Enable or disable BIOS directory options based on checkbox state
 */
void MachineConfigBios::customBiosDirToggled(bool checked)
{
    this->m_biosDirLabel->setEnabled(checked);
    this->m_biosDirLineEdit->setEnabled(checked);
    this->m_biosDirPushButton->setEnabled(checked);

    if (!checked) {
        this->m_biosDirLineEdit->clear();
    }
}

/**
 * @brief Select a directory to browse BIOS files from
 *
 * Open a file dialog to select a directory containing BIOS files
 * for the BIOS file combobox
 */
void MachineConfigBios::selectBiosFile()
{
    QString startDir = QDir(this->m_qemuObject->BIOSDirectory()).exists() ?
                      this->m_qemuObject->BIOSDirectory() :
                      QDir::homePath();

    QString biosDir = QFileDialog::getExistingDirectory(this,
                                                      tr("Select BIOS Directory"),
                                                      startDir,
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!biosDir.isEmpty()) {
        m_biosComboBox->clear();
        m_biosComboBox->addItem(tr("Default"));

        QDir dir(biosDir);
        QStringList filters;
        filters << "*.bin" << "*.rom" << "*.fd";
        QStringList biosFiles = dir.entryList(filters, QDir::Files | QDir::Readable, QDir::Name);

        for (const QString &biosFile : biosFiles) {
            m_biosComboBox->addItem(biosFile, QDir::toNativeSeparators(biosDir + QDir::separator() + biosFile));
        }

        if (m_biosComboBox->count() > 1) {
            m_biosComboBox->setCurrentIndex(1);
        }
    }
}

/**
 * @brief Select a BIOS search directory
 *
 * Open a file dialog to select a directory for QEMU's -L flag
 */
void MachineConfigBios::selectBiosDirectory()
{
    QString startDir = QDir(this->m_qemuObject->BIOSDirectory()).exists() ?
                      this->m_qemuObject->BIOSDirectory() :
                      QDir::homePath();

    QString biosDir = QFileDialog::getExistingDirectory(this,
                                                      tr("Select BIOS Search Directory"),
                                                      startDir,
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!biosDir.isEmpty()) {
        m_biosDirLineEdit->setText(QDir::toNativeSeparators(biosDir));
    }
}

/**
 * @brief Save the BIOS data
 *
 * Save the BIOS data to the machine
 */
void MachineConfigBios::saveBiosData()
{
    // Save BIOS search directory
    if (this->m_useCustomBiosDirCheckBox->isChecked() &&
        !this->m_biosDirLineEdit->text().trimmed().isEmpty()) {
        this->m_machine->setBiosDirectory(this->m_biosDirLineEdit->text().trimmed());
    } else {
        this->m_machine->setBiosDirectory("");
    }

    // Save custom BIOS file
    if (this->m_useCustomBiosCheckBox->isChecked()) {
        if (this->m_biosComboBox->currentIndex() > 0) {
            QString biosFileName = this->m_biosComboBox->currentText();
            QString biosPath;

            // Check if file exists in BIOS directory
            QString biosInDir = QDir(this->m_qemuObject->BIOSDirectory()).filePath(biosFileName);
            if (QFile::exists(biosInDir)) {
                biosPath = biosInDir;
            } else if (QFile::exists(biosFileName)) {
                biosPath = biosFileName;
            } else {
                biosPath = biosFileName;
            }

            this->m_machine->setBiosPath(biosPath);
        } else {
            this->m_machine->setBiosPath("");
        }
    } else {
        this->m_machine->setBiosPath("");
    }
}
