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
 * In this window the user can configure the BIOS of the machine
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

    m_useCustomBiosCheckBox = new QCheckBox(this);
    m_useCustomBiosCheckBox->setEnabled(enableFields);
    m_useCustomBiosCheckBox->setText(tr("Use custom BIOS"));
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
            // If file not found in the list, add it
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

    m_biosPageLayout = new QVBoxLayout();
    m_biosPageLayout->setAlignment(Qt::AlignTop);
    m_biosPageLayout->addWidget(m_useCustomBiosCheckBox);
    m_biosPageLayout->addItem(m_biosFileLayout);

    m_biosPageWidget = new QWidget(this);
    m_biosPageWidget->setLayout(m_biosPageLayout);

    // Set initial state of controls
    this->customBiosToggled(m_useCustomBiosCheckBox->isChecked());

    qDebug() << "MachineConfigBios created";
}

MachineConfigBios::~MachineConfigBios()
{
    qDebug() << "MachineConfigBios destroyed";
}

/**
 * @brief Enable or disable BIOS options
 * @param checked, true if custom BIOS is enabled
 *
 * Enable or disable BIOS options based on checkbox state
 */
void MachineConfigBios::customBiosToggled(bool checked)
{
    this->m_biosFileLabel->setEnabled(checked);
    this->m_biosComboBox->setEnabled(checked);
    this->m_biosFilePushButton->setEnabled(checked);
    
    if (!checked) {
        // If checkbox is unchecked, reset to default BIOS
        this->m_biosComboBox->setCurrentIndex(0);
    }
}

/**
 * @brief Select a BIOS directory
 *
 * Open a file dialog to select a directory containing BIOS files
 */
void MachineConfigBios::selectBiosFile()
{
    // Use home directory as default if BIOS directory doesn't exist
    QString startDir = QDir(this->m_qemuObject->BIOSDirectory()).exists() ? 
                      this->m_qemuObject->BIOSDirectory() : 
                      QDir::homePath();
                      
    QString biosDir = QFileDialog::getExistingDirectory(this, 
                                                      tr("Select BIOS Directory"),
                                                      startDir,
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!biosDir.isEmpty()) {
        // Clear the current items
        m_biosComboBox->clear();
        m_biosComboBox->addItem(tr("Default"));
        
        // Add files from the selected directory
        QDir dir(biosDir);
        QStringList filters;
        filters << "*.bin" << "*.rom" << "*.fd";
        QStringList biosFiles = dir.entryList(filters, QDir::Files | QDir::Readable, QDir::Name);
        
        for (const QString &biosFile : biosFiles) {
            m_biosComboBox->addItem(biosFile, QDir::toNativeSeparators(biosDir + QDir::separator() + biosFile));
        }
        
        // Select the first BIOS file if available
        if (m_biosComboBox->count() > 1) {
            m_biosComboBox->setCurrentIndex(1);
        }
    }
}

/**
 * @brief Save the BIOS data
 *
 * Save the BIOS data to the machine
 */
void MachineConfigBios::saveBiosData()
{
    if (this->m_useCustomBiosCheckBox->isChecked()) {
        if (this->m_biosComboBox->currentIndex() > 0) {
            QString biosFileName = this->m_biosComboBox->currentText();
            QString biosPath;
            
            // Check if file exists in BIOS directory
            QString biosInDir = QDir(this->m_qemuObject->BIOSDirectory()).filePath(biosFileName);
            if (QFile::exists(biosInDir)) {
                biosPath = biosInDir;
            } else {
                // If not, check if it's a full path
                if (QFile::exists(biosFileName)) {
                    biosPath = biosFileName;
                } else {
                    // Last resort - just use the filename and hope QEMU can find it
                    biosPath = biosFileName;
                }
            }
            
            this->m_machine->setBiosPath(biosPath);
        } else {
            // Default selected but checkbox is checked - don't set a custom BIOS
            this->m_machine->setBiosPath("");
        }
    } else {
        // Checkbox unchecked - don't use custom BIOS
        this->m_machine->setBiosPath("");
    }
}
