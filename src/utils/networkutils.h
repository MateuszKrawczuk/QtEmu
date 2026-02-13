// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QString>
#include <QStringList>
#include <QPair>
#include <QList>
#include "../networkadapter.h"

class NetworkUtils
{
public:
    static QStringList getAvailableBridges();
    static bool bridgeExists(const QString &bridgeName);
    
    static bool createBridge(const QString &bridgeName, 
                            const QString &interface,
                            QString &errorMsg);
    
    static bool canUsePkexec();
    static bool canUseBridgeHelper();
    static QString getBridgeHelperPath();
    
    static QString generateMacAddress(const QString &prefix = QStringLiteral("52:54:00"));
    static bool isValidMacAddress(const QString &mac);
    
    static QStringList getNicModelNames();
    static QString nicModelToQemu(NicModel model);
    
    static bool isBridgeSupported();
    static bool isTapSupported();
    
    static QStringList getPhysicalInterfaces();
    
    struct BridgeInfo {
        QString name;
        bool isActive;
        QStringList interfaces;
    };
    static QList<BridgeInfo> getBridgeInfoList();

private:
    NetworkUtils() = delete;
};

#endif
