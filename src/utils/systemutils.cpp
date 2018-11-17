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

// Local
#include "systemutils.h"

SystemUtils::SystemUtils() {
    qDebug() << "SystemUtils created";
}

SystemUtils::~SystemUtils() {
    qDebug() << "SystemUtils destroyed";
}

void SystemUtils::getTotalMemory(int32_t &totalRAM) {
    #ifdef Q_OS_LINUX
        struct sysinfo sys_info;

        if (sysinfo(&sys_info) != -1) {
            totalRAM = (((uint64_t)sys_info.totalram * sys_info.mem_unit) * 0.976562) / 1024 / 1024;
        }
    #endif
    #ifdef Q_OS_WIN
        MEMORYSTATUSEX statex;

        statex.dwLength = sizeof (statex);

        GlobalMemoryStatusEx (&statex);

        totalRAM = static_cast<int>(statex.ullTotalPhys / (1024*1024*1024));
    #endif
    #ifdef Q_OS_MACOS
    // TODO
    #endif
    #ifdef Q_OS_FREEBSD
    // TODO
    #endif
}

void SystemUtils::setCPUTypesx86(QComboBox *CPUType) {

    // Intel among many others...IBM, Texas Instruments, AMD, Cyrix...
    CPUType -> addItem("486", QString("486"));

    // Intel
    CPUType -> addItem("Pentium",          QString("pentium"));
    CPUType -> addItem("Pentium 2",        QString("pentium2"));
    CPUType -> addItem("Pentium 3",        QString("pentium3"));
    CPUType -> addItem("Intel Core Duo",   QString("coreduo"));
    CPUType -> addItem("Intel Core 2 Duo", QString("core2duo"));
    CPUType -> addItem("Intel Core 2 Duo P9xxx (Penryn Class Core 2)", QString("Penryn"));
    CPUType -> addItem("Intel Atom N270",  QString("n270"));
    CPUType -> addItem("Broadwell",        QString("Broadwell"));
    CPUType -> addItem("Broadwell no TSX", QString("Broadwell-noTSX"));
    CPUType -> addItem("Broadwell, IBRS",  QString("Broadwell-IBRS"));
    CPUType -> addItem("Broadwell, no TSX, IBRS", QString("Broadwell-noTSX-IBRS"));
    CPUType -> addItem("Conroe", QString("Conroe"));
    CPUType -> addItem("Haswell", QString("Haswell"));
    CPUType -> addItem("Haswell no TSX", QString("Haswell-noTSX"));
    CPUType -> addItem("Haswell, IBRS", QString("Haswell-IBRS"));
    CPUType -> addItem("Haswell, no TSX, IBRS", QString("Haswell-noTSX-IBRS"));
    CPUType -> addItem("Intel Xeon E3-12xx v2 (Ivy Bridge)", QString("IvyBridge"));
    CPUType -> addItem("Intel Xeon E3-12xx v2 (Ivy Bridge, IBRS)", QString("IvyBridge-IBRS"));
    CPUType -> addItem("Intel Core i7 9xx (Nehalem Class Core i7)", QString("Nehalem"));
    CPUType -> addItem("Intel Core i7 9xx (Nehalem Core i7, IBRS update)", QString("Nehalem-IBRS"));
    CPUType -> addItem("Intel Xeon E312xx (Sandy Bridge)", QString("SandyBridge"));
    CPUType -> addItem("Intel Xeon E312xx (Sandy Bridge, IBRS update)", QString("SandyBridge-IBRS"));
    CPUType -> addItem("Intel Core Processor (Skylake)", QString("Skylake-Client"));
    CPUType -> addItem("Intel Core Processor (Skylake, IBRS)", QString("Skylake-Client-IBRS"));
    CPUType -> addItem("Intel Xeon Processor (Skylake)", QString("Skylake-Server"));
    CPUType -> addItem("Intel Xeon Processor (Skylake, IBRS)", QString("Skylake-Server-IBRS"));
    CPUType -> addItem("Westmere E56xx/L56xx/X56xx (IBRS update)", QString("Westmere-IBRS"));
    CPUType -> addItem("Westmere E56xx/L56xx/X56xx (Nehalem-C)",   QString("Westmere"));

    // AMD
    CPUType -> addItem("AMD Opteron G1",       QString("Opteron_G1"));
    CPUType -> addItem("AMD Opteron G2",       QString("Opteron_G2"));
    CPUType -> addItem("AMD Opteron G3",       QString("Opteron_G3"));
    CPUType -> addItem("AMD Opteron G4",       QString("Opteron_G4"));
    CPUType -> addItem("AMD Opteron G5",       QString("Opteron_G5"));
    CPUType -> addItem("AMD Phenom 9550",      QString("phenom"));
    CPUType -> addItem("AMD Athlon",           QString("athlon"));
    CPUType -> addItem("AMD EPYC",             QString("EPYC"));
    CPUType -> addItem("AMD EPYC (with IBPB)", QString("EPYC-IBPB"));

    // QEMU
    CPUType -> addItem("qemu32 Virtual CPU version 2.5+",   QString("qemu32"));
    CPUType -> addItem("qemu64 Virtual CPU version 2.5+",   QString("qemu64"));
    CPUType -> addItem("kvm32 Common 32-bit KVM processor", QString("kvm32"));
    CPUType -> addItem("kvm64 Common KVM processor",        QString("kvm64"));

    // Host
    CPUType -> addItem("Base", QString("base"));
    CPUType -> addItem("Host", QString("host"));
    CPUType -> addItem("Max",  QString("max"));
}

void SystemUtils::setGPUTypes(QComboBox *GPUType) {
    GPUType -> addItem("None",                         QString("none"));
    GPUType -> addItem("Standard VGA(VESA 2.0)",       QString("std"));
    GPUType -> addItem("Cirrus Logic GD5446",          QString("cirrus"));
    GPUType -> addItem("VMWare SVGA-II",               QString("vmware"));
    GPUType -> addItem("QXL Paravirtual Graphic Card", QString("qxl"));
    GPUType -> addItem("Sun TCX Framebuffer",          QString("tcx"));
    GPUType -> addItem("Sun Cgthree Framebuffer",      QString("cg3"));
    GPUType -> addItem("Virtio VGA Card",              QString("virtio"));
    GPUType -> addItem("Xen Framebuffer",              QString("xenfb"));
}

void SystemUtils::setKeyboardLayout(QComboBox *keyboardLayout) {
    keyboardLayout -> addItem("Arabic (ar)",                      QString("ar"));
    keyboardLayout -> addItem("Danish (da)",                      QString("da"));
    keyboardLayout -> addItem("German (de)",                      QString("de"));
    keyboardLayout -> addItem("German (Switzerland) (de-ch)",     QString("de-ch"));
    keyboardLayout -> addItem("English (United Kingdom) (en-gb)", QString("en-gb"));
    keyboardLayout -> addItem("English (United States) (en-us)",  QString("en-us"));
    keyboardLayout -> addItem("Spanish (Spain) (es)",             QString("es"));
    keyboardLayout -> addItem("Estonian (et)",                    QString("et"));
    keyboardLayout -> addItem("Finnish (fi)",                     QString("fi"));
    keyboardLayout -> addItem("Faroese (fo)",                     QString("fo"));
    keyboardLayout -> addItem("French (fr)",                      QString("fr"));
    keyboardLayout -> addItem("French (Belgium) (fr-be)",         QString("fr-be"));
    keyboardLayout -> addItem("French (Canada) (fr-ca)",          QString("fr-ca"));
    keyboardLayout -> addItem("French (Switzerland) (fr-ch)",     QString("fr-ch"));
    keyboardLayout -> addItem("Croatian (hr)",                    QString("hr"));
    keyboardLayout -> addItem("Hungarian (hu)",                   QString("hu"));
    keyboardLayout -> addItem("Icelandic (is)",                   QString("is"));
    keyboardLayout -> addItem("Italian (it)",                     QString("it"));
    keyboardLayout -> addItem("Japanese (ja)",                    QString("ja"));
    keyboardLayout -> addItem("Lithuanian (lt)",                  QString("lt"));
    keyboardLayout -> addItem("Latvian (lv)",                     QString("lv"));
    keyboardLayout -> addItem("Macedonian (mk)",                  QString("mk"));
    keyboardLayout -> addItem("Dutch (nl)",                       QString("nl"));
    keyboardLayout -> addItem("Dutch (Belgium) (nl-be)",          QString("nl-be"));
    keyboardLayout -> addItem("Norwegian (no)",                   QString("no"));
    keyboardLayout -> addItem("Polish (pl)",                      QString("pl"));
    keyboardLayout -> addItem("Portuguese (pt)",                  QString("pt"));
    keyboardLayout -> addItem("Portuguese (Brazil) (pt-br)",      QString("pt-br"));
    keyboardLayout -> addItem("Russian (ru)",                     QString("ru"));
    keyboardLayout -> addItem("Slovenian (sl)",                   QString("sl"));
    keyboardLayout -> addItem("Swedish (sv)",                     QString("sv"));
    keyboardLayout -> addItem("Thai (th)",                        QString("th"));
    keyboardLayout -> addItem("Turkish (tr)",                     QString("tr"));
}

QHash<QString, QString> SystemUtils::getSoundCards() {

    QHash<QString, QString> soundCardsHash;

    soundCardsHash.insert("sb16", "Creative Sound Blaster 16");
    soundCardsHash.insert("ac97", "Intel AC97(82801AA)");
    soundCardsHash.insert("gus", "Gravis Ultrasound GF1");
    soundCardsHash.insert("hda", "Intel HD Audio");
    soundCardsHash.insert("es1370", "ENSONIQ AudioPCI ES1370");
    soundCardsHash.insert("adlib", "Yamaha YM3812");
    soundCardsHash.insert("cs4231a", "CS4231A");
    soundCardsHash.insert("pcspk", "PC Speaker");

    return soundCardsHash;
}

QHash<QString, QString> SystemUtils::getAccelerators() {

    QHash<QString, QString> acceleratorsHash;

#ifdef Q_OS_LINUX
    acceleratorsHash.insert("kvm", "Kernel-based Virtual Machine (KVM)");
#endif
    acceleratorsHash.insert("xen", "Xen Hypervisor");
    acceleratorsHash.insert("tcg", "Tiny Code Generator (TCG)");
#ifdef Q_OS_WIN
    acceleratorsHash.insert("hax", "Hardware Accelerated Execution Manager (HAXM)");
#endif

    return acceleratorsHash;
}

QMap<QString, QString> SystemUtils::getMediaDevices() {

    QMap<QString, QString> mediaMap;

    mediaMap.insert("a", "Floppy A");
    mediaMap.insert("b", "Floppy B");
    mediaMap.insert("c", "HDD");
    mediaMap.insert("d", "CDROM");
    mediaMap.insert("n-1", "Network 1");
    mediaMap.insert("n-2", "Network 2");

    return mediaMap;
}

QString SystemUtils::getOsIcon(const QString &osVersion) {

    if (osVersion.contains("Microsoft", Qt::CaseInsensitive)) {
        if (osVersion.contains("7", Qt::CaseInsensitive) ||
            osVersion.contains("8", Qt::CaseInsensitive) ||
            osVersion.contains("10", Qt::CaseInsensitive)) {
            return "windows";
        } else {
            return "windows_old";
        }
    } else if (osVersion.contains("GNU", Qt::CaseInsensitive)) {
        return "gnu";
    } else {
        return osVersion.toLower().replace(" ", "_");
    }
}

bool SystemUtils::createDisk(QEMU *qemuGlobalObject,
                             const QString &diskName,
                             const QString &format,
                             const double size,
                             bool useEncryption) {

    QProcess *qemuImgProcess = new QProcess();

    QStringList args;
    args << "create";

    if(useEncryption) {
        args << "-e";
    }

    args << "-f";
    args << format;
    args << diskName;
    args << QString::number(size) + "G"; // TODO: Implement other sizes, K, M, T

    QString program;

    program = qemuGlobalObject -> QEMUImgPath();

    qemuImgProcess -> start(program, args);

    if( ! qemuImgProcess -> waitForStarted(2000)) {
        QMessageBox *qemuImgNotFoundMessageBox = new QMessageBox();
        qemuImgNotFoundMessageBox -> setWindowTitle(QObject::tr("Qtemu - Critical error"));
        qemuImgNotFoundMessageBox -> setIcon(QMessageBox::Critical);
        qemuImgNotFoundMessageBox -> setText(QObject::tr("<p>Cannot start qemu-img</p>"
                                                         "<p><strong>Image isn't created</strong></p>"
                                                         "<p>Ensure that you have installed qemu-img in your "
                                                         "system and it's available</p>"));
        qemuImgNotFoundMessageBox -> exec();

        return false;
    }

    if( ! qemuImgProcess -> waitForFinished()) {
        QMessageBox *qemuImgNotFinishedMessageBox = new QMessageBox();
        qemuImgNotFinishedMessageBox -> setWindowTitle(QObject::tr("Qtemu - Critical error"));
        qemuImgNotFinishedMessageBox -> setIcon(QMessageBox::Critical);
        qemuImgNotFinishedMessageBox -> setText(QObject::tr("<p>Cannot finish qemu-img</p>"
                                                            "<p><strong>Image isn't created</strong></p>"
                                                            "<p>There's a problem with qemu-img, the process "
                                                            "has not finished correctly</p>"));
        qemuImgNotFinishedMessageBox -> exec();

        return false;
    } else {
        QByteArray err = qemuImgProcess -> readAllStandardError();
        QByteArray out = qemuImgProcess -> readAllStandardOutput();

        if(err.count() > 0) {
            QMessageBox *qemuImgErrorMessageBox = new QMessageBox();
            qemuImgErrorMessageBox -> setWindowTitle(QObject::tr("Qtemu - Critical error"));
            qemuImgErrorMessageBox -> setIcon(QMessageBox::Critical);
            qemuImgErrorMessageBox -> setText(QObject::tr("<p>Cannot finish qemu-img</p>"
                                                          "<p><strong>Image isn't created</strong></p>"
                                                          "<p>Error: " + err + "</p>"));
            qemuImgErrorMessageBox -> exec();

            return false;
        }

        if(out.count() > 0) {
            QMessageBox *qemuImgOkMessageBox = new QMessageBox();
            qemuImgOkMessageBox -> setWindowTitle(QObject::tr("Qtemu - Image created"));
            qemuImgOkMessageBox -> setIcon(QMessageBox::Information);
            qemuImgOkMessageBox -> setText(QObject::tr("<p><strong>Image created</strong></p>"
                                                       "<p>" + out + "</p>"));
            qemuImgOkMessageBox -> exec();

            return true;
        }

        return false;
    }
}
