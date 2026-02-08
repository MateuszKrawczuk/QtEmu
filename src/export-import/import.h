// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef IMPORT_H
#define IMPORT_H

// Qt
#include <QWizard>
#include <QListWidget>

#include <QDebug>

// Local
#include "../machine.h"
#include "importgeneralpage.h"
#include "importdestinationpage.h"
#include "importdetailspage.h"
#include "importmediapage.h"

class ImportWizard : public QWizard {
    Q_OBJECT

    public:
        explicit ImportWizard(Machine *machine,
                              QListWidget *osListWidget,
                              QWidget *parent = nullptr);
        ~ImportWizard();

        enum { Page_General, Page_Destination, Page_Details, Page_Media };

    signals:

    public slots:

    protected:

    private:

};

#endif // IMPORT_H
