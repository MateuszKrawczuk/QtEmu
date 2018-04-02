/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef CONCLUSIONPAGE_H
#define CONCLUSIONPAGE_H

// Qt
#include <QWizard>
#include <QLabel>
#include <QGridLayout>
#include <QSettings>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QListWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUuid>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"
#include "../utils/logger.h"

class MachineConclusionPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineConclusionPage(Machine *machine, QListWidget *osListWidget,
                                       QWidget *parent = 0);
        ~MachineConclusionPage();

    signals:

    public slots:

    protected:

    private:
        void initializePage();
        bool validatePage();
        bool createDisk(const QString &format, const double size, bool useEncryption);
        void createMachineJSON(Machine *machine) const;
        void fillMachineJSON(QJsonObject &machineJSONObject) const;
        void populateOSListJSON();

        QGridLayout *conclusionLayout;

        QLabel *machineDescLabel;
        QLabel *OSTypeDescLabel;
        QLabel *OSVersionDescLabel;
        QLabel *processorDescLabel;
        QLabel *graphicsDescLabel;
        QLabel *audioDescLabel;
        QLabel *RAMDescLabel;
        QLabel *acceleratorDescLabel;
        QLabel *diskDescLabel;

        QLabel *conclusionLabel;
        QLabel *machineNameLabel;
        QLabel *OSTypeLabel;
        QLabel *OSVersionLabel;
        QLabel *processorLabel;
        QLabel *graphicsLabel;
        QLabel *audioLabel;
        QLabel *RAMLabel;
        QLabel *acceleratorLabel;
        QLabel *diskLabel;

        QMessageBox *qemuImgNotFoundMessageBox;
        QMessageBox *qemuImgNotFinishedMessageBox;
        QMessageBox *qemuImgErrorMessageBox;
        QMessageBox *qemuImgOkMessageBox;

        QProcess *qemuImgProcess;

        QListWidget *osList;

        Machine *newMachine;
};


#endif // CONCLUSIONPAGE_H
