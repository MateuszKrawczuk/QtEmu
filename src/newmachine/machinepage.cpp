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
    QRegExp regExp(this->filterLineEdit->text());
    customFilter->setFilterRegExp(regExp);
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

    this->addMachine(model, "none", "empty machine");
    this->addMachine(model, "isapc", "ISA-only PC");
    this->addMachine(model, "pc", "Standard PC (i440FX + PIIX, 1996) (alias of pc-i440fx-3.0)");
    this->addMachine(model, "pc-i440fx-3.0", "Standard PC (i440FX + PIIX, 1996) (default)");
    this->addMachine(model, "pc-i440fx-2.12", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.11", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.10", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.9", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.8", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.7", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.6", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.5", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.4", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.3", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.2", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.1", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-2.0", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.7", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.6", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.5", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-i440fx-1.4", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.3", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.2", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.1", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.1", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-1.0", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.15", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.14", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.13", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.12", "Standard PC (i440FX + PIIX, 1996)");
    this->addMachine(model, "pc-0.11", "Standard PC (i440FX + PIIX, 1996) (deprecated)");
    this->addMachine(model, "pc-0.10", "Standard PC (i440FX + PIIX, 1996) (deprecated)");
    this->addMachine(model, "q35", "Standard PC (Q35 + ICH9, 2009) (alias of pc-q35-3.0)");
    this->addMachine(model, "pc-q35-3.0", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.12", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.11", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.10", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.9", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.8", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.7", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.6", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.5", "Standard PC (Q35 + ICH9, 2009)");
    this->addMachine(model, "pc-q35-2.4", "Standard PC (Q35 + ICH9, 2009)");

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
