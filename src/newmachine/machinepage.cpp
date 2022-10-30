/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
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
#include "machinepage.h"

/**
 * @brief Machine page
 * @param machine, new machine object
 * @param parent, widget parent
 *
 * Machine page section. In this page you can select the machine type.
 * ex: for x86_64, pc-i440fx-1.4
 */
MachinePage::MachinePage(Machine *machine,
                         QWidget *parent) : QWizardPage(parent)
{
    this->setTitle(tr("Machine page"));
    this->m_newMachine = machine;

    customFilter = new CustomFilter(this);
    this->setMachines();

    filterLabel = new QLabel(tr("Filter:"));

    filterLineEdit = new QLineEdit();
    connect(filterLineEdit, &QLineEdit::textChanged,
            this, &MachinePage::textFilterChanged);

    machinesTypesTreeView = new QTreeView;
    machinesTypesTreeView->setRootIsDecorated(false);
    machinesTypesTreeView->setAlternatingRowColors(true);
    machinesTypesTreeView->setModel(customFilter);
    machinesTypesTreeView->setSortingEnabled(true);
    machinesTypesTreeView->sortByColumn(1, Qt::DescendingOrder);

    machinePageLayout = new QGridLayout;
    machinePageLayout->addWidget(filterLabel, 0, 0);
    machinePageLayout->addWidget(filterLineEdit, 0, 1);
    machinePageLayout->addWidget(machinesTypesTreeView, 1, 0, 1, 3);

    this->setLayout(machinePageLayout);

    qDebug() << "MachinePage created";
}

MachinePage::~MachinePage()
{
    qDebug() << "MachinePage destroyed";
}

bool MachinePage::validatePage()
{
    QModelIndexList indexes = machinesTypesTreeView->selectionModel()->selectedIndexes();
    if (indexes.size() > 0) {
        QModelIndex selectedIndex = indexes.at(0);

        QString machineType = selectedIndex.data().toString();

        if (!machineType.isEmpty() && machineType != "none") {
            this->m_newMachine->setType(machineType);
        }
    }

    return true;
}

void MachinePage::textFilterChanged()
{
    QRegularExpression regExp(this->filterLineEdit->text());
    customFilter->setFilterRegularExpression(regExp);
}

/**
 * @brief Add the machines to the model
 *
 * Add all the machines to the model
 */
void MachinePage::setMachines()
{
    QStandardItemModel *model = new QStandardItemModel(0, 2, this);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Machine"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Description"));


    //Better reduce number of version machine
    //Later it should parse result from qemu-system-x86_64.exe

    this->addMachine(model, "none", "empty machine");
    this->addMachine(model, "isapc", "ISA-only PC");
    this->addMachine(model, "pc", "Standard PC (i440FX + PIIX, 1996) (alias of pc-i440fx-7.1)");
    this->addMachine(model, "q35", "Standard PC (Q35 + ICH9, 2009) (alias of pc-q35-7.1)");
    this->addMachine(model, "microvm", "microvm (i386)");

    customFilter->setSourceModel(model);
}

/**
 * @brief Add the machine to the model
 * @param model, model list with the machines
 * @param machine, code of the machine
 * @param description, description of the machine
 *
 * Add the machine to the model
 */
void MachinePage::addMachine(QAbstractItemModel *model,
                             const QString &machine,
                             const QString &description)
{
    model->insertRow(0);
    model->setData(model->index(0, 0), machine);
    model->setData(model->index(0, 1), description);
}
