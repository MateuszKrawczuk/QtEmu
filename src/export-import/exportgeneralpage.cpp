// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "exportgeneralpage.h"

ExportGeneralPage::ExportGeneralPage(QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine export wizard"));

    m_infoLabel = new QLabel(tr("Select the folder where the machine has to be exported."));

    m_destinationLineEdit = new QLineEdit();
    m_destinationButton = new QPushButton(QIcon::fromTheme("folder-symbolic",
                                                           QIcon(QPixmap(":/images/icons/breeze/32x32/folder-symbolic.svg"))),
                                          "",
                                          this);

    connect(m_destinationButton, &QAbstractButton::clicked,
            this, &ExportGeneralPage::selectExportDestination);

    m_destinationLayout = new QHBoxLayout();
    m_destinationLayout->addWidget(m_destinationLineEdit);
    m_destinationLayout->addWidget(m_destinationButton);

    this->registerField("destination*", m_destinationLineEdit);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_infoLabel);
    m_mainLayout->addItem(m_destinationLayout);

    this->setLayout(m_mainLayout);

    qDebug() << "ExportGeneralPage created";
}

ExportGeneralPage::~ExportGeneralPage()
{
    qDebug() << "ExportGeneralPage destroyed";
}

/**
 * @brief Select the destination of the machine
 *
 * Select the destination folder for the machine
 */
void ExportGeneralPage::selectExportDestination()
{
    QString exportPath = QFileDialog::getExistingDirectory(this, tr("Select the export folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly |
                                                           QFileDialog::DontResolveSymlinks
                                                           );
    if (!exportPath.isEmpty()) {
        this->m_destinationLineEdit->setText(QDir::toNativeSeparators(exportPath));
    }
}
