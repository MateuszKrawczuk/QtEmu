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
#include "exportdetailspage.h"

ExportDetailsPage::ExportDetailsPage(Machine *machine,
                                     QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine details"));

    m_machineDetailsTree = new QTreeWidget(this);
    m_machineDetailsTree->setColumnCount(1);
    m_machineDetailsTree->setHeaderHidden(true);
    m_machineDetailsTree->setRootIsDecorated(false);
    m_machineDetailsTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addWidget(m_machineDetailsTree);

    this->setLayout(m_mainLayout);

    qDebug() << "ExportDetailsPage created";
}

ExportDetailsPage::~ExportDetailsPage()
{
    qDebug() << "ExportDetailsPage destroyed";
}
