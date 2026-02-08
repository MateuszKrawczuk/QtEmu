// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "importgeneralpage.h"

ImportGeneralPage::ImportGeneralPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine import wizard"));

    m_infoLabel = new QLabel(tr("Select the machine configuration file."));

    m_machineConfigLineEdit = new QLineEdit();
    m_machineConfigButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                          "",
                                          this);

    connect(m_machineConfigButton, &QAbstractButton::clicked,
            this, &ImportGeneralPage::selectMachineConfigFile);

    m_machineConfigLayout = new QHBoxLayout();
    m_machineConfigLayout->addWidget(m_machineConfigLineEdit);
    m_machineConfigLayout->addWidget(m_machineConfigButton);

    this->registerField("configFilePath*", m_machineConfigLineEdit);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addItem(m_machineConfigLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ImportGeneralPage created";
}

ImportGeneralPage::~ImportGeneralPage()
{
    qDebug() << "ImportGeneralPage destroyed";
}

/**
 * @brief Select the machine config file
 *
 * Select the machine config file
 */
void ImportGeneralPage::selectMachineConfigFile()
{
    QString machineConfigFile = QFileDialog::getOpenFileName(this,
                                                             tr("Open machine config file"),
                                                             QDir::homePath(),
                                                             tr("Config file (*.json)"));

    if (!machineConfigFile.isEmpty()) {
        this->m_machineConfigLineEdit->setText(QDir::toNativeSeparators(machineConfigFile));
    }
}
