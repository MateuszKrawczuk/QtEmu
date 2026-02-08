// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
#include <QStringList>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

// Local
#include "../machine.h"
#include "../machinewizard.h"
#include "../utils/systemutils.h"

class MachineDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineDiskPage(Machine *machine,
                                 QWidget *parent = nullptr);
        ~MachineDiskPage();

    signals:

    public slots:
        void useExistingDiskToggle(bool toggled);
        void useExistingDiskPath();

    protected:

    private:
        int nextId() const override;
        bool validatePage() override;

        QVBoxLayout *m_machineDiskLayout;
        QHBoxLayout *m_useOldDiskLayout;

        QRadioButton *m_noDiskRadio;
        QRadioButton *m_createDiskRadio;
        QRadioButton *m_useExistingDiskRadio;

        QPushButton *m_pathNewDiskPushButton;

        QLineEdit *m_hardDiskPathLineEdit;

        QLabel *m_machineDiskLabel;
        QLabel *m_machineDiskInfoLabel;
        QLabel *m_machineDiskSizeLabel;

        QMessageBox *m_notDiskMessageBox;

        QString m_existingDiskPath;

        Machine *m_newMachine;
};

class MachineNewDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNewDiskPage(Machine *machine,
                                    QWidget *parent = nullptr);
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

        QVBoxLayout *m_newDiskLayout;
        QHBoxLayout *m_fileLocationLayout;
        QGridLayout *m_fileSizeLayout;
        QGridLayout *m_diskTypeLayout;

        QGroupBox *m_fileLocationGroupBox;
        QGroupBox *m_fileSizeGroupBox;
        QGroupBox *m_fileTypeGroupBox;

        QLineEdit *m_fileNameLineEdit;
        QLineEdit *m_diskFormatLineEdit; // Its hidden - used to share data between QWizardPages

        QDoubleSpinBox *m_diskSpinBox;

        QSlider *m_diskSlider;

        QLabel *m_minDiskLabel;
        QLabel *m_maxDisklabel;
        QLabel *m_memoryLabel;

        QPushButton *m_pathNewDiskPushButton;

        QRadioButton *m_rawRadioButton;
        QRadioButton *m_qcowRadioButton;
        QRadioButton *m_qcow2RadioButton;
        QRadioButton *m_qedRadioButton;
        QRadioButton *m_vmdkRadioButton;
        QRadioButton *m_cloopRadioButton;

        QMessageBox *m_qemuImgNotFoundMessageBox;

        QString m_diskName;

        Machine *m_newMachine;
};

#endif // DISKPAGE_H
