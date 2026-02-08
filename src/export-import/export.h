// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef EXPORT_H
#define EXPORT_H

// Qt
#include <QWizard>

#include <QDebug>

// Local
#include "../machine.h"
#include "exportgeneralpage.h"
#include "exportdetailspage.h"
#include "exportmediapage.h"

class ExportWizard : public QWizard {
    Q_OBJECT

    public:
        explicit ExportWizard(Machine *machine,
                              QWidget *parent = nullptr);
        ~ExportWizard();

        enum { Page_General, Page_Details, Page_Media };

    signals:

    public slots:

    protected:

    private:

};

#endif // EXPORT_H
