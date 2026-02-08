// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "importdestinationpage.h"

ImportDestinationPage::ImportDestinationPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine import wizard"));

    m_infoLabel = new QLabel(tr("Select the folder where the machine has to be imported."));
    m_infoLabel->setWordWrap(true);

    m_machineDestinationLineEdit = new QLineEdit();
    m_machineDestinationButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                                 "",
                                                 this);

    connect(m_machineDestinationButton, &QAbstractButton::clicked,
            this, &ImportDestinationPage::selectMachineDestination);

    m_machineDestinationLayout = new QHBoxLayout();
    m_machineDestinationLayout->addWidget(m_machineDestinationLineEdit);
    m_machineDestinationLayout->addWidget(m_machineDestinationButton);

    this->registerField("machineDestinationPath*", m_machineDestinationLineEdit);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addItem(m_machineDestinationLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ImportDestinationPage created";
}

ImportDestinationPage::~ImportDestinationPage()
{
    qDebug() << "ImportDestinationPage destroyed";
}

/**
 * @brief Select the machine destination
 *
 * Select the machine destination
 */
void ImportDestinationPage::selectMachineDestination()
{
    QString exportPath = QFileDialog::getExistingDirectory(this, tr("Select the import folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly |
                                                           QFileDialog::DontResolveSymlinks
                                                           );

    if (!exportPath.isEmpty()) {
        this->m_machineDestinationLineEdit->setText(QDir::toNativeSeparators(exportPath));
    }
}
