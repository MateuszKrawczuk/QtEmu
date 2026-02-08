// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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

    return (sourceModel()->data(code).toString().contains(filterRegularExpression()) ||
            sourceModel()->data(description).toString().contains(filterRegularExpression()));
}
