// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "networkutils.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QDebug>

#ifdef Q_OS_LINUX
#include <sys/stat.h>
#endif

QStringList NetworkUtils::getAvailableBridges()
{
    QStringList bridges;

#ifdef Q_OS_LINUX
    QDir bridgeDir(QStringLiteral("/sys/class/net"));
    QStringList interfaces = bridgeDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QString &iface : interfaces) {
        QString bridgePath = QStringLiteral("/sys/class/net/%1/bridge").arg(iface);
        if (QDir(bridgePath).exists()) {
            bridges.append(iface);
        }
    }
#elif defined(Q_OS_MACOS)
    QProcess process;
    process.start(QStringLiteral("ifconfig"), QStringList());
    process.waitForFinished(5000);
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    
    QRegularExpression bridgeRegex(QStringLiteral("^([a-zA-Z0-9]+):\\s+flags=.*<[^>]*BRIDGE[^>]*>"),
                                   QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator it = bridgeRegex.globalMatch(output);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        bridges.append(match.captured(1));
    }
#elif defined(Q_OS_WIN)
    // On Windows, bridges are typically Hyper-V switches
    // This is a placeholder - full implementation would use PowerShell/HNS
    QProcess process;
    process.start(QStringLiteral("powershell"), 
                  QStringList() << QStringLiteral("-Command") 
                  << QStringLiteral("Get-VMSwitch | Select-Object -ExpandProperty Name"));
    process.waitForFinished(10000);
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QStringList lines = output.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (!trimmed.isEmpty()) {
            bridges.append(trimmed);
        }
    }
#endif

    return bridges;
}

bool NetworkUtils::bridgeExists(const QString &bridgeName)
{
    return getAvailableBridges().contains(bridgeName);
}

bool NetworkUtils::createBridge(const QString &bridgeName,
                                const QString &interface,
                                QString &errorMsg)
{
    if (bridgeName.isEmpty()) {
        errorMsg = QStringLiteral("Bridge name cannot be empty");
        return false;
    }

    if (bridgeExists(bridgeName)) {
        errorMsg = QStringLiteral("Bridge '%1' already exists").arg(bridgeName);
        return false;
    }

    if (!canUsePkexec()) {
        errorMsg = QStringLiteral("pkexec is not available. Install polkit.");
        return false;
    }

#ifdef Q_OS_LINUX
    QProcess process;
    QStringList args;
    
    // Create the bridge
    args << QStringLiteral("ip") << QStringLiteral("link") << QStringLiteral("add") 
         << QStringLiteral("name") << bridgeName << QStringLiteral("type") << QStringLiteral("bridge");
    
    process.start(QStringLiteral("pkexec"), args);
    process.waitForFinished(30000);
    
    if (process.exitCode() != 0) {
        errorMsg = QString::fromLocal8Bit(process.readAllStandardError());
        return false;
    }
    
    // Bring up the bridge
    args.clear();
    args << QStringLiteral("ip") << QStringLiteral("link") << QStringLiteral("set") 
         << bridgeName << QStringLiteral("up");
    
    process.start(QStringLiteral("pkexec"), args);
    process.waitForFinished(30000);
    
    if (process.exitCode() != 0) {
        errorMsg = QString::fromLocal8Bit(process.readAllStandardError());
        return false;
    }
    
    // Add interface to bridge if specified
    if (!interface.isEmpty()) {
        args.clear();
        args << QStringLiteral("ip") << QStringLiteral("link") << QStringLiteral("set") 
             << interface << QStringLiteral("master") << bridgeName;
        
        process.start(QStringLiteral("pkexec"), args);
        process.waitForFinished(30000);
        
        if (process.exitCode() != 0) {
            errorMsg = QStringLiteral("Bridge created but failed to add interface: ") 
                      + QString::fromLocal8Bit(process.readAllStandardError());
            return false;
        }
    }
    
    // Add to QEMU bridge config
    QString bridgeConfigPath = QStringLiteral("/etc/qemu/bridge.conf");
    args.clear();
    args << QStringLiteral("sh") << QStringLiteral("-c") 
         << QStringLiteral("echo 'allow %1' >> %2").arg(bridgeName, bridgeConfigPath);
    
    process.start(QStringLiteral("pkexec"), args);
    process.waitForFinished(30000);
    
    return true;
#elif defined(Q_OS_MACOS)
    QProcess process;
    QStringList args;
    
    args << QStringLiteral("ifconfig") << bridgeName << QStringLiteral("create");
    
    process.start(QStringLiteral("pkexec"), args);
    process.waitForFinished(30000);
    
    if (process.exitCode() != 0) {
        errorMsg = QString::fromLocal8Bit(process.readAllStandardError());
        return false;
    }
    
    if (!interface.isEmpty()) {
        args.clear();
        args << QStringLiteral("ifconfig") << bridgeName << QStringLiteral("addm") << interface;
        
        process.start(QStringLiteral("pkexec"), args);
        process.waitForFinished(30000);
        
        if (process.exitCode() != 0) {
            errorMsg = QString::fromLocal8Bit(process.readAllStandardError());
            return false;
        }
    }
    
    return true;
#elif defined(Q_OS_WIN)
    // Windows uses Hyper-V switches
    QProcess process;
    QString script = QStringLiteral("New-VMSwitch -Name '%1' -SwitchType External").arg(bridgeName);
    
    if (!interface.isEmpty()) {
        script = QStringLiteral("New-VMSwitch -Name '%1' -NetAdapterName '%2'").arg(bridgeName, interface);
    }
    
    process.start(QStringLiteral("powershell"), 
                  QStringList() << QStringLiteral("-Command") << QStringLiteral("Start-Process powershell -Verb RunAs -ArgumentList '-Command %1'").arg(script));
    process.waitForFinished(30000);
    
    return process.exitCode() == 0;
#else
    errorMsg = QStringLiteral("Bridge creation not supported on this platform");
    return false;
#endif
}

bool NetworkUtils::canUsePkexec()
{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start(QStringLiteral("which"), QStringList(QStringLiteral("pkexec")));
    process.waitForFinished(5000);
    return process.exitCode() == 0;
#else
    return false;
#endif
}

bool NetworkUtils::canUseBridgeHelper()
{
    QString helperPath = getBridgeHelperPath();
    if (helperPath.isEmpty()) {
        return false;
    }
    
#ifdef Q_OS_LINUX
    struct stat st;
    if (stat(helperPath.toLocal8Bit().constData(), &st) == 0) {
        // Check if setuid bit is set
        return (st.st_mode & S_ISUID) != 0;
    }
#endif
    return false;
}

QString NetworkUtils::getBridgeHelperPath()
{
    QStringList possiblePaths = {
        QStringLiteral("/usr/lib/qemu/qemu-bridge-helper"),
        QStringLiteral("/usr/libexec/qemu-bridge-helper"),
        QStringLiteral("/usr/local/lib/qemu/qemu-bridge-helper"),
        QStringLiteral("/usr/lib/qemu-bridge-helper")
    };
    
    for (const QString &path : possiblePaths) {
        if (QFile::exists(path)) {
            return path;
        }
    }
    
    return QString();
}

QString NetworkUtils::generateMacAddress(const QString &prefix)
{
    QString mac = prefix;
    QRandomGenerator *rng = QRandomGenerator::global();
    
    for (int i = 0; i < 3; ++i) {
        mac += QLatin1Char(':');
        int byte = rng->bounded(256);
        mac += QStringLiteral("%1").arg(byte, 2, 16, QLatin1Char('0'));
    }
    
    return mac.toLower();
}

bool NetworkUtils::isValidMacAddress(const QString &mac)
{
    QRegularExpression regex(QStringLiteral("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$"));
    return regex.match(mac).hasMatch();
}

QStringList NetworkUtils::getNicModelNames()
{
    return NetworkAdapter::availableNicModelNames();
}

QString NetworkUtils::nicModelToQemu(NicModel model)
{
    return NetworkAdapter::nicModelToString(model);
}

bool NetworkUtils::isBridgeSupported()
{
#ifdef Q_OS_LINUX
    return true;
#elif defined(Q_OS_WIN)
    // Check if Hyper-V is available
    QProcess process;
    process.start(QStringLiteral("powershell"), 
                  QStringList() << QStringLiteral("-Command") 
                  << QStringLiteral("Get-Command Get-VMSwitch -ErrorAction SilentlyContinue"));
    process.waitForFinished(10000);
    return process.exitCode() == 0;
#else
    return false;
#endif
}

bool NetworkUtils::isTapSupported()
{
#ifdef Q_OS_LINUX
    return QFile::exists(QStringLiteral("/dev/net/tun"));
#else
    return false;
#endif
}

QStringList NetworkUtils::getPhysicalInterfaces()
{
    QStringList interfaces;

#ifdef Q_OS_LINUX
    QDir netDir(QStringLiteral("/sys/class/net"));
    QStringList allInterfaces = netDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    
    for (const QString &iface : allInterfaces) {
        // Skip bridges, loopback, and virtual interfaces
        QString bridgePath = QStringLiteral("/sys/class/net/%1/bridge").arg(iface);
        QString tunPath = QStringLiteral("/sys/class/net/%1/tun_flags").arg(iface);
        
        if (!QDir(bridgePath).exists() && !QFile::exists(tunPath) && 
            iface != QStringLiteral("lo") && !iface.startsWith(QStringLiteral("veth")) &&
            !iface.startsWith(QStringLiteral("docker")) && !iface.startsWith(QStringLiteral("virbr"))) {
            interfaces.append(iface);
        }
    }
#elif defined(Q_OS_MACOS)
    QProcess process;
    process.start(QStringLiteral("ifconfig"), QStringList());
    process.waitForFinished(5000);
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    
    QRegularExpression ifaceRegex(QStringLiteral("^([a-zA-Z0-9]+):\\s+flags=.*status: (active|inactive)"),
                                  QRegularExpression::MultilineOption);
    QRegularExpressionMatchIterator it = ifaceRegex.globalMatch(output);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        QString name = match.captured(1);
        if (name != QStringLiteral("lo0") && !name.startsWith(QStringLiteral("bridge")) &&
            !name.startsWith(QStringLiteral("utun")) && !name.startsWith(QStringLiteral("awdl"))) {
            interfaces.append(name);
        }
    }
#elif defined(Q_OS_WIN)
    QProcess process;
    process.start(QStringLiteral("powershell"), 
                  QStringList() << QStringLiteral("-Command") 
                  << QStringLiteral("Get-NetAdapter -Physical | Select-Object -ExpandProperty Name"));
    process.waitForFinished(10000);
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    interfaces = output.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    for (int i = 0; i < interfaces.size(); ++i) {
        interfaces[i] = interfaces[i].trimmed();
    }
#endif

    return interfaces;
}

QList<NetworkUtils::BridgeInfo> NetworkUtils::getBridgeInfoList()
{
    QList<BridgeInfo> result;

#ifdef Q_OS_LINUX
    QStringList bridges = getAvailableBridges();
    
    for (const QString &bridgeName : bridges) {
        BridgeInfo info;
        info.name = bridgeName;
        
        // Check if bridge is up
        QString operstatePath = QStringLiteral("/sys/class/net/%1/operstate").arg(bridgeName);
        QFile operstateFile(operstatePath);
        if (operstateFile.open(QIODevice::ReadOnly)) {
            QString state = QString::fromLocal8Bit(operstateFile.readAll().trimmed());
            info.isActive = (state == QStringLiteral("up"));
            operstateFile.close();
        }
        
        // Get interfaces on this bridge
        QString ifacesPath = QStringLiteral("/sys/class/net/%1/brif").arg(bridgeName);
        QDir ifacesDir(ifacesPath);
        info.interfaces = ifacesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        
        result.append(info);
    }
#endif

    return result;
}
