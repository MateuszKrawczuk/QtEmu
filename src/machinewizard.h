/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
 * Copyright (C) 2017 Sergio Carlavilla <carlavilla @ mailbox.org>
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

#ifndef MACHINEWIZARD_H
#define MACHINEWIZARD_H

// Qt
#include <QWizard>
#include <QSettings>
#include <QDir>
#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTextBrowser>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QJsonObject>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

#include <QDebug>

// Local
#include "machine.h"
#include "systemutils.h"

class MachineWizard : public QWizard {
    Q_OBJECT

    public:
        explicit MachineWizard(Machine *machine,
                               QWidget *parent = 0);
        ~MachineWizard();

    enum { Page_Name, Page_Hardware, Page_Accelerator,
           Page_Memory, Page_Disk, Page_New_Disk };

        void createMachineJSON(QJsonObject &machineJson) const;

    signals:

    public slots:

    protected:

    private:

};

class MachineNamePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineNamePage(Machine *machine,
                                 QWidget *parent = 0);
        ~MachineNamePage();

    signals:

    public slots:
        void selectOS(int OSSelected);

    protected:

    private:
        bool validatePage();
        void cleanupPage();

        QString machineFolderCreated;

        QGridLayout *mainLayout;

        QLabel *descriptionNameLabel;
        QLabel *machineNameLabel;
        QLabel *OSTypeLabel;
        QLabel *OSVersionLabel;

        QComboBox *OSType;
        QComboBox *OSVersion;

        QLineEdit *machineNameLineEdit;

        QMessageBox *createMachineMessageBox;

        Machine *newMachine;
};

class MachineHardwarePage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineHardwarePage(Machine *machine,
                                     QWidget *parent = 0);
        ~MachineHardwarePage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *hardwareLayout;

        QTabWidget *hardwareTabWidget;

        Machine *newMachine;
};

class ProcessorTab: public QWidget {
    Q_OBJECT

    public:
        explicit ProcessorTab(Machine *machine,
                              QWidget *parent = 0);
        ~ProcessorTab();

    signals:

    public slots:
        void selectProcessor(const QString &processor);
        void selectCPUCount(int CPUCount);
        void selectSocketCount(int socketCount);
        void selectCoresSocket(int coresSocket);
        void selectThreadsCore(int threadsCore);
        void selectMaxHotCPU(int maxCPU);

    protected:

    private:
        QHBoxLayout *CPUTypeLayout;
        QHBoxLayout *CPUCountLayout;
        QHBoxLayout *socketLayout;
        QHBoxLayout *maxHotCPUsLayout;
        QVBoxLayout *CPUSettingsLayout;
        QVBoxLayout *processorLayout;

        QComboBox *CPUType;

        QGroupBox *CPUSettings;

        QLabel *CPUTypeLabel;
        QLabel *CPUCountLabel;
        QLabel *coresSocketLabel;
        QLabel *socketCountLabel;
        QLabel *threadsCoreLabel;
        QLabel *maxHotCPULabel;

        QSpinBox *CPUCountSpinBox;
        QSpinBox *coresSocketSpinBox;
        QSpinBox *socketCountSpinBox;
        QSpinBox *threadsCoreSpinBox;
        QSpinBox *maxHotCPUSpinBox;

        Machine *newMachine;
};

class GraphicsTab: public QWidget {
    Q_OBJECT

    public:
        explicit GraphicsTab(Machine *machine,
                             QWidget *parent = 0);
        ~GraphicsTab();
    signals:

    public slots:
        void selectGraphics(const QString &GPUType);
        void selectKeyboard(const QString &keyboard);

    protected:

    private:
        QGridLayout *graphicsLayout;

        QComboBox *GPUType;
        QComboBox *keyboard;

        QLabel *GPUTypeLabel;
        QLabel *keyboardLabel;

        Machine *newMachine;
};

class AudioTab: public QWidget {
    Q_OBJECT

    public:
        explicit AudioTab(Machine *machine,
                          QWidget *parent = 0);
        ~AudioTab();
    signals:

    public slots:
        void selectCreativeCard(bool selectCreative);
        void selectEnsoniqCard(bool selectEnsoniq);
        void selectIntelAC97Card(bool selectIntelAC97);
        void selectYamahaCard(bool selectYamaha);
        void selectGravisCard(bool selectGravis);
        void selectCS4231ACard(bool selectCS4231A);
        void selectIntelHDCard(bool selectIntelHD);
        void selectPCSpeakerCard(bool selectPCSpeaker);

    protected:

    private:
        QGridLayout *audioLayout;

        QCheckBox *creativeCheck;
        QCheckBox *ensoniqCheck;
        QCheckBox *intelAC97Check;
        QCheckBox *yamahaCheck;
        QCheckBox *gravisCheck;
        QCheckBox *CS4231ACheck;
        QCheckBox *intelHDACheck;
        QCheckBox *pcSpeakerCheck;

        Machine *newMachine;
};

class NetworkTab: public QWidget {
    Q_OBJECT

    public:
        explicit NetworkTab(Machine *machine,
                            QWidget *parent = 0);
        ~NetworkTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *networkLayout;

        QRadioButton *withNetworkRadio;
        QRadioButton *withoutNetworkRadio;

        Machine *newMachine;
};

class MachineAcceleratorPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineAcceleratorPage(Machine *machine,
                                        QWidget *parent = 0);
        ~MachineAcceleratorPage();

    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *acceleratorLayout;

        QTabWidget *acceleratorTabWidget;

        Machine *newMachine;
};

class KVMTab: public QWidget {
    Q_OBJECT

    public:
        explicit KVMTab(Machine *machine,
                        QWidget *parent = 0);
        ~KVMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *kvmLayout;

        QRadioButton *kvmRadioButton;

        QLabel *kvmDescriptionLabel;
        QLabel *kvmURLLabel;

        Machine *newMachine;
};

class TCGTab: public QWidget {
    Q_OBJECT

    public:
        explicit TCGTab(Machine *machine,
                        QWidget *parent = 0);
        ~TCGTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *tcgLayout;

        QRadioButton *tcgRadioButton;

        QLabel *tcgDescriptionLabel;
        QLabel *tcgURLLabel;

        Machine *newMachine;
};

class HAXMTab: public QWidget {
    Q_OBJECT

    public:
        explicit HAXMTab(Machine *machine,
                         QWidget *parent = 0);
        ~HAXMTab();
    signals:

    public slots:

    protected:

    private:
        QVBoxLayout *haxmLayout;

        QRadioButton *haxmRadioButton;

        QLabel *haxmDescriptionLabel;
        QLabel *haxmURLLabel;

        Machine *newMachine;
};

class MachineMemoryPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineMemoryPage(Machine *machine,
                                   QWidget *parent = 0);
        ~MachineMemoryPage();

    signals:

    public slots:

    protected:

    private:
        QGridLayout *machineMemoryLayout;

        QSpinBox *memorySpinBox;
        QSlider *memorySlider;

        QLabel *descriptionMemoryLabel;
        QLabel *spinBoxMemoryLabel;
        QLabel *minMemoryLabel;
        QLabel *maxMemorylabel;

        Machine *newMachine;
};

class MachineDiskPage: public QWizardPage {
    Q_OBJECT

    public:
        explicit MachineDiskPage(Machine *machine,
                                 QWidget *parent = 0);
        ~MachineDiskPage();

    signals:

    public slots:
        void useExistingDiskToggle(bool toggled);

    protected:

    private:
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

    protected:

    private:
        void initializePage();

        QVBoxLayout *newDiskLayout;
        QHBoxLayout *fileLocationLayout;
        QGridLayout *fileSizeLayout;
        QGridLayout *diskTypeLayout;

        QGroupBox *fileLocationGroupBox;
        QGroupBox *fileSizeGroupBox;
        QGroupBox *fileTypeGroupBox;

        QLineEdit *fileName;

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

        Machine *newMachine;
};

#endif // MACHINEWIZARD_H
