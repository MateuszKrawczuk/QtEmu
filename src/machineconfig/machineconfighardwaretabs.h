/*
 * This file is part of QtEmu project.
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
#ifndef MACHINECONFIGHARDWARETABS_H
#define MACHINECONFIGHARDWARETABS_H

// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QSpinBox>

// Local
#include "../machine.h"

class ProcessorConfigTab: public QWidget {
    Q_OBJECT

    public:
        explicit ProcessorConfigTab(Machine *machine,
                                    QWidget *parent = nullptr);
        ~ProcessorConfigTab();

        // Methods
        QString getCPUType();
        int getCPUCount();
        int getCoresSocket();
        int getSocketCount();
        int getThreadsCore();
        int getMaxHotCPU();

    signals:

    public slots:

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

};

class GraphicsConfigTab: public QWidget {
    Q_OBJECT

    public:
        explicit GraphicsConfigTab(Machine *machine,
                                    QWidget *parent = nullptr);
        ~GraphicsConfigTab();

        // Methods
        QString getGPUType();
        QString getKeyboardLayout();

    signals:

    public slots:

    protected:

    private:
        QGridLayout *m_graphicsLayout;

        QComboBox *m_GPUType;
        QComboBox *m_keyboard;

        QLabel *m_GPUTypeLabel;
        QLabel *m_keyboardLabel;

};

#endif // MACHINECONFIGHARDWARETABS_H
