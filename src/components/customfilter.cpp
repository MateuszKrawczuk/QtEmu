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
#include "customfilter.h"


CustomFilter::CustomFilter(QObject *parent) : QSortFilterProxyModel(parent)
{
    qDebug() << "CustomFilter created";
}

CustomFilter::~CustomFilter()
{
    qDebug() << "CustomFilter destroyed";
}

/**
 * @brief Override the flags function to disable the edition
 *
 * Override the flags function to disable the edition
 */
Qt::ItemFlags CustomFilter::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}

bool CustomFilter::filterAcceptsRow(int row, const QModelIndex &sourceParent) const
{
    QModelIndex code = sourceModel()->index(row, 0, sourceParent);
    QModelIndex description = sourceModel()->index(row, 1, sourceParent);

    return (sourceModel()->data(code).toString().contains(filterRegExp()) ||
            sourceModel()->data(description).toString().contains(filterRegExp()));
}
