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

#ifndef DISKPAGE_H
#define DISKPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QProcess>
#include <QStringList>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

// Local
#include "../machine.h"
#include "../machinewizard.h"
#include "../utils/systemutils.h"

class MachineDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineDiskPage(Machine *machine,
                                 QWidget *parent = 0);
        ~MachineDiskPage();

    signals:

    public slots:
        void useExistingDiskToggle(bool toggled);
        void useExistingDiskPath();

    protected:

    private:
        int nextId() const override;
        bool validatePage();

        QVBoxLayout *machineDiskLayout;
        QHBoxLayout *useOldDiskLayout;

        QRadioButton *noDiskRadio;
        QRadioButton *createDiskRadio;
        QRadioButton *useExistingDiskRadio;

        QPushButton *pathNewDiskPushButton;

        QLineEdit *hardDiskPathLineEdit;

        QLabel *machineDiskLabel;
        QLabel *machineDiskInfoLabel;
        QLabel *machineDiskSizeLabel;

        QMessageBox *notDiskMessageBox;

        QString existingDiskPath;

        Machine *newMachine;
};

class MachineNewDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNewDiskPage(Machine *machine,
                                    QWidget *parent = 0);
        ~MachineNewDiskPage();

    signals:

    public slots:
        void selectRawFormat(bool useRaw);
        void selectQCowFormat(bool useQCow);
        void selectQCow2Format(bool useQCow2);
        void selectQedFormat(bool useQed);
        void selectVmdkFormat(bool useVmdk);
        void selectCloopFormat(bool useCloop);
        void selectNameNewDisk();

    protected:

    private:
        void initializePage();
        bool validatePage();

        QVBoxLayout *newDiskLayout;
        QHBoxLayout *fileLocationLayout;
        QGridLayout *fileSizeLayout;
        QGridLayout *diskTypeLayout;

        QGroupBox *fileLocationGroupBox;
        QGroupBox *fileSizeGroupBox;
        QGroupBox *fileTypeGroupBox;

        QLineEdit *fileNameLineEdit;

        QDoubleSpinBox *diskSpinBox;

        QSlider *diskSlider;

        QLabel *minDiskLabel;
        QLabel *maxDisklabel;
        QLabel *memoryLabel;

        QPushButton *pathNewDiskPushButton;

        QRadioButton *rawRadioButton;
        QRadioButton *qcowRadioButton;
        QRadioButton *qcow2RadioButton;
        QRadioButton *qedRadioButton;
        QRadioButton *vmdkRadioButton;
        QRadioButton *cloopRadioButton;

        QProcess *qemuImgProcess;

        QMessageBox *qemuImgNotFoundMessageBox;
        QMessageBox *qemuImgNotFinishedMessageBox;
        QMessageBox *qemuImgErrorMessageBox;
        QMessageBox *qemuImgOkMessageBox;

        QString diskFormat;
        QString diskName;

        Machine *newMachine;
};

#endif // DISKPAGE_H
