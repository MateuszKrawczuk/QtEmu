/*
 * This file is part of QtEmu project.
 * Copyright (C) 2006-2009 Urs Wolfer <uwolfer @ fwo.ch> and Ben Klopfenstein <benklop gmail com>
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

#ifndef HARDWAREPAGE_H
#define HARDWAREPAGE_H

// Qt
#include <QWizard>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>

// Local
#include "../machine.h"
#include "../utils/systemutils.h"

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
        QVBoxLayout *m_hardwareLayout;

        QTabWidget *m_hardwareTabWidget;

        Machine *m_newMachine;
};

class ProcessorTab: public QWidget {
    Q_OBJECT

    public:
        explicit ProcessorTab(Machine *machine,
                              QWidget *parent = 0);
        ~ProcessorTab();

    signals:

    public slots:
        void selectProcessor(int index);
        void selectCPUCount(int CPUCount);
        void selectSocketCount(int socketCount);
        void selectCoresSocket(int coresSocket);
        void selectThreadsCore(int threadsCore);
        void selectMaxHotCPU(int maxCPU);

    protected:

    private:
        QHBoxLayout *m_CPUTypeLayout;
        QHBoxLayout *m_CPUCountLayout;
        QHBoxLayout *m_socketLayout;
        QHBoxLayout *m_maxHotCPUsLayout;
        QVBoxLayout *m_CPUSettingsLayout;
        QVBoxLayout *m_processorLayout;

        QComboBox *m_CPUType;

        QGroupBox *m_CPUSettings;

        QLabel *m_CPUTypeLabel;
        QLabel *m_CPUCountLabel;
        QLabel *m_coresSocketLabel;
        QLabel *m_socketCountLabel;
        QLabel *m_threadsCoreLabel;
        QLabel *m_maxHotCPULabel;

        QSpinBox *m_CPUCountSpinBox;
        QSpinBox *m_coresSocketSpinBox;
        QSpinBox *m_socketCountSpinBox;
        QSpinBox *m_threadsCoreSpinBox;
        QSpinBox *m_maxHotCPUSpinBox;

        Machine *m_newMachine;
};

class GraphicsTab: public QWidget {
    Q_OBJECT

    public:
        explicit GraphicsTab(Machine *machine,
                             QWidget *parent = 0);
        ~GraphicsTab();
    signals:

    public slots:
        void selectGraphics(int index);
        void selectKeyboard(int index);

    protected:

    private:
        QGridLayout *m_graphicsLayout;

        QComboBox *m_GPUType;
        QComboBox *m_keyboard;

        QLabel *m_GPUTypeLabel;
        QLabel *m_keyboardLabel;

        Machine *m_newMachine;
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
        QGridLayout *m_audioLayout;

        QCheckBox *m_creativeCheck;
        QCheckBox *m_ensoniqCheck;
        QCheckBox *m_intelAC97Check;
        QCheckBox *m_yamahaCheck;
        QCheckBox *m_gravisCheck;
        QCheckBox *m_CS4231ACheck;
        QCheckBox *m_intelHDACheck;
        QCheckBox *m_pcSpeakerCheck;

        Machine *m_newMachine;
};

class NetworkTab: public QWidget {
    Q_OBJECT

    public:
        explicit NetworkTab(Machine *machine,
                            QWidget *parent = 0);
        ~NetworkTab();
    signals:

    public slots:
        void networkState(bool network);

    protected:

    private:
        QVBoxLayout *m_networkLayout;

        QRadioButton *m_withNetworkRadio;
        QRadioButton *m_withoutNetworkRadio;

        Machine *m_newMachine;
};

#endif // HARDWAREPAGE_H
