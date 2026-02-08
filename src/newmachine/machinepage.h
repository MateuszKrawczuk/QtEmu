// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MACHINEPAGE_H
#define MACHINEPAGE_H

// Qt
#include <QWizard>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QGridLayout>

// Local
#include "../components/customfilter.h"
#include "../machine.h"

class MachinePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachinePage(Machine *machine,
                             QWidget *parent = nullptr);
        ~MachinePage();

    signals:

    public slots:

    private slots:
        void textFilterChanged();

    protected:

    private:
        QGridLayout *machinePageLayout;

        CustomFilter *customFilter;

        QTreeView *machinesTypesTreeView;

        QLabel *filterLabel;

        QLineEdit *filterLineEdit;

        Machine *m_newMachine;

        // Methods
        bool validatePage();

        void setMachines();
        void addMachine(QAbstractItemModel *model, const QString &machine, const QString &description);
};

#endif // MACHINEPAGE_H
