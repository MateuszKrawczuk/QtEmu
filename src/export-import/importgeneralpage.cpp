/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2019 Sergio Carlavilla <carlavilla @ mailbox.org>
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
