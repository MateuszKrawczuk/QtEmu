// SPDX-FileCopyrightText: 2006-2009 Urs Wolfer <uwolfer @ fwo.ch>
// SPDX-FileCopyrightText: 2006-2009 Ben Klopfenstein <benklop gmail com>
// SPDX-FileCopyrightText: 2017-2020 Sergio Carlavilla <carlavilla @ mailbox.org>
// SPDX-License-Identifier: GPL-2.0-or-later

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
    qtemuApp.setApplicationVersion("2.2");
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
              << QLibraryInfo::path(QLibraryInfo::TranslationsPath).toStdString()
              << "/"
              << languageFile.toStdString()
              << "\n";

    languageLoaded = translatorQt.load(languageFile, QLibraryInfo::path(QLibraryInfo::TranslationsPath));

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
