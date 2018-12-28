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

// Qt
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>
#include <QCoreApplication>

// C++ standard library
#include <iostream>

// Local
#include "mainwindow.h"
#include "qemu.h"
#include "utils/logger.h"
#include "utils/firstrunwizard.h"

int main(int argc, char *argv[])
{
    QApplication qtemuApp(argc, argv);
    qtemuApp.setApplicationName("QtEmu");
    qtemuApp.setApplicationVersion("2.0");
    qtemuApp.setOrganizationName("QtEmu");
    qtemuApp.setOrganizationDomain("https://www.qtemu.org");

    std::cout << QString("QtEmu v%1 # QtEmu Developers")
                        .arg(qtemuApp.applicationVersion()).toStdString();

    std::cout << QString("- Built with Qt v%1").arg(QT_VERSION_STR)
                                               .toStdString();

    /*
     * REPRODUCIBLEBUILD is defined via .pro file when SOURCE_DATE_EPOCH is
     * defined in the build environment. This is used to avoid hardcoding
     * timestamps and this way make the builds reproducible.
     */
    #ifndef REPRODUCIBLEBUILD
        std::cout << QString(" on %1, %2")
                     .arg(__DATE__)
                     .arg(__TIME__).toStdString();
    #endif

    QSettings settings;

    // Data folder
    settings.beginGroup("DataFolder");
    QDir dataDirectory;
    QString dataDirectoryPath = QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/");
    QString dataDirectoryLogs = QDir::toNativeSeparators(dataDirectoryPath + "logs");

    if (!dataDirectory.exists(dataDirectoryPath)) {
        dataDirectory.mkdir(dataDirectoryPath);

        if (!dataDirectory.exists(dataDirectoryLogs)) {
            dataDirectory.mkdir(dataDirectoryLogs);
        }
    }

    settings.setValue("QtEmuData", dataDirectoryPath);
    settings.setValue("QtEmuLogs", dataDirectoryLogs);
    settings.endGroup();

    // Translations
    QTranslator translatorQt;
    QTranslator translatorQtEmu;
    QString languageFile;
    QString language;
    bool languageLoaded;

    settings.beginGroup("Configuration");
    language = settings.value("language", "en").toString();
    settings.endGroup();

    qDebug() << "The language loaded" << language;

    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    languageFile = QString("qt_%1").arg(language);

    std::cout << "\n"
              << "Using Qt translation "
              << QLibraryInfo::location(QLibraryInfo::TranslationsPath).toStdString()
              << "/"
              << languageFile.toStdString()
              << "\n";

    languageLoaded = translatorQt.load(languageFile, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    if (languageLoaded) {
        std::cout << "Language loaded";
        qtemuApp.installTranslator(&translatorQt);
    } else {
        std::cout << "Language unavailable";
    }

    languageFile = QString(":/translations/qtemu_%1").arg(language);

    std::cout << "\n"
              << "Using QtEmu translation "
              << languageFile.toStdString()
              << "\n";

    languageLoaded = translatorQtEmu.load(languageFile);

    if (languageLoaded) {
        std::cout << "Language loaded";
        qtemuApp.installTranslator(&translatorQtEmu);
    } else {
        std::cout << "Language unavailable";
    }

    // Launch first run winzard
    settings.beginGroup("Configuration");
    bool runFirstRunWizard = settings.value("firstrunwizard", true).toBool();
    settings.endGroup();
    settings.sync(); // sync settings

    if (runFirstRunWizard) {
        FirstRunWizard *firstRunWizard = new FirstRunWizard(nullptr);
        firstRunWizard->show();
        firstRunWizard->exec();

        delete firstRunWizard;
    }

    // Load Logger
    QString logMessage = "QtEmu started with PID\t";
    logMessage.append(QString::number(QCoreApplication::applicationPid()));

    Logger::logQtemuAction(logMessage);

    std::cout << "\n";
    std::cout << QString("- Running with Qt v%1\n\n").arg(qVersion())
                                                     .toStdString();
    std::cout.flush();

    MainWindow qtemuWindow;
    qtemuWindow.show();

    return qtemuApp.exec();
}
