// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CUSTOMFILTER_H
#define CUSTOMFILTER_H

// Qt
#include <QSortFilterProxyModel>
#include <QMap>

#include <QDebug>

class CustomFilter: public QSortFilterProxyModel {
    Q_OBJECT

    public:
        CustomFilter(QObject *parent = nullptr);
        ~CustomFilter() override;

        Qt::ItemFlags flags(const QModelIndex &index) const override;

    protected:
        bool filterAcceptsRow(int row, const QModelIndex &sourceParent) const override;

    private:

};

#endif // CUSTOMFILTER_H
