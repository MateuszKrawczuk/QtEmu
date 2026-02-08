// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "machineconfigcloudinit.h"

/**
 * @brief Cloud-init configuration window
 * @param machine, machine to be configured
 * @param parent, parent widget
 *
 * In this window the user can configure cloud-init settings for the machine
 */
MachineConfigCloudInit::MachineConfigCloudInit(Machine *machine,
                                               QWidget *parent) : QWidget(parent)
{
    bool enableFields = true;

    if (machine->getState() != Machine::Stopped) {
        enableFields = false;
    }

    this->m_machine = machine;

    // Enable cloud-init checkbox
    m_enableCloudInitCheckBox = new QCheckBox(this);
    m_enableCloudInitCheckBox->setEnabled(enableFields);
    m_enableCloudInitCheckBox->setText(tr("Enable cloud-init"));
    m_enableCloudInitCheckBox->setChecked(this->m_machine->getCloudInitEnabled());
    connect(m_enableCloudInitCheckBox, &QAbstractButton::toggled,
            this, &MachineConfigCloudInit::cloudInitToggled);

    // Help label
    m_helpLabel = new QLabel(tr("Cloud-init allows automatic configuration of virtual machines. "
                               "It configures hostname, users, SSH keys, and more at first boot."), this);
    m_helpLabel->setWordWrap(true);
    m_helpLabel->setStyleSheet("QLabel { color: gray; font-size: 9pt; }");

    // Basic configuration group
    m_basicConfigGroupBox = new QGroupBox(tr("Basic Configuration"), this);
    m_basicConfigGroupBox->setEnabled(m_enableCloudInitCheckBox->isChecked() && enableFields);

    m_hostnameLabel = new QLabel(tr("Hostname") + ":", this);
    m_hostnameLineEdit = new QLineEdit(this);
    m_hostnameLineEdit->setText(this->m_machine->getCloudInitHostname());
    m_hostnameLineEdit->setPlaceholderText(tr("Enter hostname for the VM"));

    m_usernameLabel = new QLabel(tr("Username") + ":", this);
    m_usernameLineEdit = new QLineEdit(this);
    m_usernameLineEdit->setText(this->m_machine->getCloudInitUsername());
    m_usernameLineEdit->setPlaceholderText(tr("Default user (e.g., ubuntu)"));

    m_passwordLabel = new QLabel(tr("Password") + ":", this);
    m_passwordLineEdit = new QLineEdit(this);
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);
    m_passwordLineEdit->setText(this->m_machine->getCloudInitPassword());
    m_passwordLineEdit->setPlaceholderText(tr("User password (optional if SSH key is provided)"));

    QLabel *passwordSecurityNote = new QLabel(
        tr("<i><b>Security Note:</b> Password is hashed using SHA-512 and NOT saved in configuration file. "
           "You will need to re-enter it if you edit cloud-init settings later.</i>"), this);
    passwordSecurityNote->setWordWrap(true);
    passwordSecurityNote->setStyleSheet("QLabel { color: #666666; font-size: 8pt; margin-left: 20px; }");

    m_sshKeyLabel = new QLabel(tr("SSH Public Key") + ":", this);
    m_sshKeyLineEdit = new QLineEdit(this);
    m_sshKeyLineEdit->setText(this->m_machine->getCloudInitSSHKey());
    m_sshKeyLineEdit->setPlaceholderText(tr("SSH public key or path to .pub file"));

    m_sshKeyBrowseButton = new QPushButton(this);
    m_sshKeyBrowseButton->setIcon(QIcon::fromTheme("folder-symbolic",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))));
    m_sshKeyBrowseButton->setToolTip(tr("Browse for SSH public key file"));
    connect(m_sshKeyBrowseButton, &QAbstractButton::clicked,
            this, &MachineConfigCloudInit::selectSSHKeyFile);

    m_sshKeyLayout = new QHBoxLayout();
    m_sshKeyLayout->addWidget(m_sshKeyLineEdit);
    m_sshKeyLayout->addWidget(m_sshKeyBrowseButton);

    m_basicConfigLayout = new QFormLayout();
    m_basicConfigLayout->addRow(m_hostnameLabel, m_hostnameLineEdit);
    m_basicConfigLayout->addRow(m_usernameLabel, m_usernameLineEdit);
    m_basicConfigLayout->addRow(m_passwordLabel, m_passwordLineEdit);
    m_basicConfigLayout->addRow("", passwordSecurityNote);
    m_basicConfigLayout->addRow(m_sshKeyLabel, m_sshKeyLayout);

    m_basicConfigGroupBox->setLayout(m_basicConfigLayout);

    // Advanced configuration group
    m_advancedConfigGroupBox = new QGroupBox(tr("Advanced Configuration (Optional)"), this);
    m_advancedConfigGroupBox->setEnabled(m_enableCloudInitCheckBox->isChecked() && enableFields);

    m_userDataLabel = new QLabel(tr("Custom user-data (YAML format):"), this);
    m_userDataTextEdit = new QTextEdit(this);
    m_userDataTextEdit->setPlainText(this->m_machine->getCloudInitUserData());
    m_userDataTextEdit->setPlaceholderText(tr("#cloud-config\n# Add custom cloud-init configuration here"));
    m_userDataTextEdit->setMinimumHeight(150);

    m_loadUserDataButton = new QPushButton(tr("Load from file..."), this);
    m_loadUserDataButton->setIcon(QIcon::fromTheme("document-open",
                                                   QIcon(QPixmap(":/images/icons/breeze/32x32/document-open.svg"))));
    connect(m_loadUserDataButton, &QAbstractButton::clicked,
            this, &MachineConfigCloudInit::loadUserDataFile);

    m_userDataButtonsLayout = new QHBoxLayout();
    m_userDataButtonsLayout->addStretch();
    m_userDataButtonsLayout->addWidget(m_loadUserDataButton);

    QVBoxLayout *advancedLayout = new QVBoxLayout();
    advancedLayout->addWidget(m_userDataLabel);
    advancedLayout->addWidget(m_userDataTextEdit);
    advancedLayout->addLayout(m_userDataButtonsLayout);

    m_advancedConfigGroupBox->setLayout(advancedLayout);

    // Main layout
    m_cloudInitPageLayout = new QVBoxLayout();
    m_cloudInitPageLayout->setAlignment(Qt::AlignTop);
    m_cloudInitPageLayout->addWidget(m_enableCloudInitCheckBox);
    m_cloudInitPageLayout->addWidget(m_helpLabel);
    m_cloudInitPageLayout->addSpacing(10);
    m_cloudInitPageLayout->addWidget(m_basicConfigGroupBox);
    m_cloudInitPageLayout->addWidget(m_advancedConfigGroupBox);
    m_cloudInitPageLayout->addStretch();

    m_cloudInitPageWidget = new QWidget(this);
    m_cloudInitPageWidget->setLayout(m_cloudInitPageLayout);

    qDebug() << "MachineConfigCloudInit created";
}

MachineConfigCloudInit::~MachineConfigCloudInit()
{
    qDebug() << "MachineConfigCloudInit destroyed";
}

/**
 * @brief Enable or disable cloud-init options
 * @param checked, true if cloud-init is enabled
 *
 * Enable or disable cloud-init options based on checkbox state
 */
void MachineConfigCloudInit::cloudInitToggled(bool checked)
{
    this->m_basicConfigGroupBox->setEnabled(checked);
    this->m_advancedConfigGroupBox->setEnabled(checked);
}

/**
 * @brief Select an SSH public key file
 *
 * Open a file dialog to select an SSH public key file
 */
void MachineConfigCloudInit::selectSSHKeyFile()
{
    QString sshKeyFile = QFileDialog::getOpenFileName(this,
                                                      tr("Select SSH Public Key"),
                                                      QDir::homePath() + "/.ssh",
                                                      tr("Public Key Files (*.pub);;All Files (*)"));
    if (!sshKeyFile.isEmpty()) {
        QFile file(sshKeyFile);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString keyContent = QString::fromUtf8(file.readAll()).trimmed();
            file.close();

            // Validate that it looks like an SSH key
            if (keyContent.startsWith("ssh-rsa") ||
                keyContent.startsWith("ssh-ed25519") ||
                keyContent.startsWith("ecdsa-sha2") ||
                keyContent.startsWith("ssh-dss")) {
                m_sshKeyLineEdit->setText(keyContent);
            } else {
                QMessageBox::warning(this, tr("Invalid SSH Key"),
                                   tr("The selected file does not appear to contain a valid SSH public key."));
            }
        } else {
            QMessageBox::warning(this, tr("Error"),
                               tr("Could not open the selected file."));
        }
    }
}

/**
 * @brief Load user-data from a file
 *
 * Open a file dialog to load custom user-data YAML
 */
void MachineConfigCloudInit::loadUserDataFile()
{
    QString userDataFile = QFileDialog::getOpenFileName(this,
                                                        tr("Load user-data File"),
                                                        QDir::homePath(),
                                                        tr("YAML Files (*.yml *.yaml);;All Files (*)"));
    if (!userDataFile.isEmpty()) {
        QFile file(userDataFile);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromUtf8(file.readAll());
            file.close();
            m_userDataTextEdit->setPlainText(content);
        } else {
            QMessageBox::warning(this, tr("Error"),
                               tr("Could not open the selected file."));
        }
    }
}

/**
 * @brief Save the cloud-init data
 *
 * Save the cloud-init data to the machine
 */
void MachineConfigCloudInit::saveCloudInitData()
{
    this->m_machine->setCloudInitEnabled(this->m_enableCloudInitCheckBox->isChecked());
    this->m_machine->setCloudInitHostname(this->m_hostnameLineEdit->text().trimmed());
    this->m_machine->setCloudInitUsername(this->m_usernameLineEdit->text().trimmed());
    this->m_machine->setCloudInitPassword(this->m_passwordLineEdit->text());
    this->m_machine->setCloudInitSSHKey(this->m_sshKeyLineEdit->text().trimmed());
    this->m_machine->setCloudInitUserData(this->m_userDataTextEdit->toPlainText());
}
