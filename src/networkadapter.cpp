// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "networkadapter.h"
#include <QRandomGenerator>

NetworkAdapter::NetworkAdapter(QObject *parent)
    : QObject(parent)
    , m_backend(NetworkBackend::User)
    , m_nicModel(NicModel::Virtio)
    , m_bootROM(false)
{
}

NetworkAdapter::~NetworkAdapter()
{
}

QString NetworkAdapter::id() const
{
    return m_id;
}

void NetworkAdapter::setId(const QString &id)
{
    m_id = id;
}

NetworkBackend NetworkAdapter::backend() const
{
    return m_backend;
}

void NetworkAdapter::setBackend(NetworkBackend backend)
{
    m_backend = backend;
}

NicModel NetworkAdapter::nicModel() const
{
    return m_nicModel;
}

void NetworkAdapter::setNicModel(NicModel model)
{
    m_nicModel = model;
}

QString NetworkAdapter::macAddress() const
{
    return m_macAddress;
}

void NetworkAdapter::setMacAddress(const QString &mac)
{
    m_macAddress = mac;
}

QString NetworkAdapter::bridgeName() const
{
    return m_bridgeName;
}

void NetworkAdapter::setBridgeName(const QString &name)
{
    m_bridgeName = name;
}

QString NetworkAdapter::tapDevice() const
{
    return m_tapDevice;
}

void NetworkAdapter::setTapDevice(const QString &device)
{
    m_tapDevice = device;
}

QString NetworkAdapter::socketListen() const
{
    return m_socketListen;
}

void NetworkAdapter::setSocketListen(const QString &address)
{
    m_socketListen = address;
}

QString NetworkAdapter::socketConnect() const
{
    return m_socketConnect;
}

void NetworkAdapter::setSocketConnect(const QString &address)
{
    m_socketConnect = address;
}

bool NetworkAdapter::bootROM() const
{
    return m_bootROM;
}

void NetworkAdapter::setBootROM(bool enabled)
{
    m_bootROM = enabled;
}

QList<PortForward> NetworkAdapter::portForwards() const
{
    return m_portForwards;
}

void NetworkAdapter::setPortForwards(const QList<PortForward> &forwards)
{
    m_portForwards = forwards;
}

void NetworkAdapter::addPortForward(const PortForward &forward)
{
    m_portForwards.append(forward);
}

void NetworkAdapter::removePortForward(int index)
{
    if (index >= 0 && index < m_portForwards.size()) {
        m_portForwards.removeAt(index);
    }
}

QJsonObject NetworkAdapter::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["backend"] = backendToString(m_backend);
    json["nicModel"] = nicModelToString(m_nicModel);
    json["macAddress"] = m_macAddress;
    json["bridgeName"] = m_bridgeName;
    json["tapDevice"] = m_tapDevice;
    json["socketListen"] = m_socketListen;
    json["socketConnect"] = m_socketConnect;
    json["bootROM"] = m_bootROM;

    QJsonArray portForwardsArray;
    for (const PortForward &pf : m_portForwards) {
        QJsonObject pfJson;
        pfJson["protocol"] = pf.protocol;
        pfJson["hostPort"] = pf.hostPort;
        pfJson["guestPort"] = pf.guestPort;
        pfJson["hostAddress"] = pf.hostAddress;
        pfJson["guestAddress"] = pf.guestAddress;
        portForwardsArray.append(pfJson);
    }
    json["portForwards"] = portForwardsArray;

    return json;
}

NetworkAdapter* NetworkAdapter::fromJson(const QJsonObject &json, QObject *parent)
{
    NetworkAdapter *adapter = new NetworkAdapter(parent);
    adapter->setId(json["id"].toString());
    adapter->setBackend(stringToBackend(json["backend"].toString()));
    adapter->setNicModel(stringToNicModel(json["nicModel"].toString()));
    adapter->setMacAddress(json["macAddress"].toString());
    adapter->setBridgeName(json["bridgeName"].toString());
    adapter->setTapDevice(json["tapDevice"].toString());
    adapter->setSocketListen(json["socketListen"].toString());
    adapter->setSocketConnect(json["socketConnect"].toString());
    adapter->setBootROM(json["bootROM"].toBool());

    QJsonArray portForwardsArray = json["portForwards"].toArray();
    for (const QJsonValue &value : portForwardsArray) {
        QJsonObject pfJson = value.toObject();
        PortForward pf;
        pf.protocol = pfJson["protocol"].toString();
        pf.hostPort = pfJson["hostPort"].toInt();
        pf.guestPort = pfJson["guestPort"].toInt();
        pf.hostAddress = pfJson["hostAddress"].toString();
        pf.guestAddress = pfJson["guestAddress"].toString();
        adapter->addPortForward(pf);
    }

    return adapter;
}

QString NetworkAdapter::toNetdevCommand() const
{
    if (m_backend == NetworkBackend::None) {
        return QString();
    }

    QString cmd = QString("%1,id=%2").arg(backendToNetdevType(m_backend), m_id);

    switch (m_backend) {
    case NetworkBackend::User: {
        QStringList hostfwdList;
        for (const PortForward &pf : m_portForwards) {
            QString hostfwd = QString("%1:").arg(pf.protocol);
            if (!pf.hostAddress.isEmpty()) {
                hostfwd += pf.hostAddress + ":";
            }
            hostfwd += QString("%1-").arg(pf.hostPort);
            if (!pf.guestAddress.isEmpty()) {
                hostfwd += pf.guestAddress + ":";
            }
            hostfwd += QString::number(pf.guestPort);
            hostfwdList.append(hostfwd);
        }
        if (!hostfwdList.isEmpty()) {
            cmd += ",hostfwd=" + hostfwdList.join(",");
        }
        break;
    }
    case NetworkBackend::Bridge:
        if (!m_bridgeName.isEmpty()) {
            cmd += QString(",br=%1").arg(m_bridgeName);
        }
        break;
    case NetworkBackend::Tap:
        if (!m_tapDevice.isEmpty()) {
            cmd += QString(",ifname=%1,script=no,downscript=no").arg(m_tapDevice);
        } else {
            cmd += ",script=no,downscript=no";
        }
        break;
    case NetworkBackend::Socket:
        if (!m_socketListen.isEmpty()) {
            cmd += QString(",listen=%1").arg(m_socketListen);
        } else if (!m_socketConnect.isEmpty()) {
            cmd += QString(",connect=%1").arg(m_socketConnect);
        }
        break;
    default:
        break;
    }

    return cmd;
}

QString NetworkAdapter::toDeviceCommand() const
{
    QString deviceName = nicModelToQemuDevice(m_nicModel);
    QString cmd = QString("%1,netdev=%2").arg(deviceName, m_id);

    if (!m_macAddress.isEmpty()) {
        cmd += QString(",mac=%1").arg(m_macAddress);
    }

    return cmd;
}

QString NetworkAdapter::backendToString(NetworkBackend backend)
{
    switch (backend) {
    case NetworkBackend::None: return "none";
    case NetworkBackend::User: return "user";
    case NetworkBackend::Bridge: return "bridge";
    case NetworkBackend::Tap: return "tap";
    case NetworkBackend::Socket: return "socket";
    default: return "user";
    }
}

NetworkBackend NetworkAdapter::stringToBackend(const QString &str)
{
    if (str == "none") return NetworkBackend::None;
    if (str == "user") return NetworkBackend::User;
    if (str == "bridge") return NetworkBackend::Bridge;
    if (str == "tap") return NetworkBackend::Tap;
    if (str == "socket") return NetworkBackend::Socket;
    return NetworkBackend::User;
}

QString NetworkAdapter::nicModelToString(NicModel model)
{
    switch (model) {
    case NicModel::Virtio: return "virtio";
    case NicModel::E1000: return "e1000";
    case NicModel::E1000E: return "e1000e";
    case NicModel::Rtl8139: return "rtl8139";
    case NicModel::Pcnet: return "pcnet";
    case NicModel::I82551: return "i82551";
    case NicModel::I82557B: return "i82557b";
    case NicModel::I82559er: return "i82559er";
    default: return "virtio";
    }
}

NicModel NetworkAdapter::stringToNicModel(const QString &str)
{
    if (str == "virtio") return NicModel::Virtio;
    if (str == "e1000") return NicModel::E1000;
    if (str == "e1000e") return NicModel::E1000E;
    if (str == "rtl8139") return NicModel::Rtl8139;
    if (str == "pcnet") return NicModel::Pcnet;
    if (str == "i82551") return NicModel::I82551;
    if (str == "i82557b") return NicModel::I82557B;
    if (str == "i82559er") return NicModel::I82559er;
    return NicModel::Virtio;
}

QStringList NetworkAdapter::availableNicModelNames()
{
    return {
        QStringLiteral("virtio"),
        QStringLiteral("e1000"),
        QStringLiteral("e1000e"),
        QStringLiteral("rtl8139"),
        QStringLiteral("pcnet"),
        QStringLiteral("i82551"),
        QStringLiteral("i82557b"),
        QStringLiteral("i82559er")
    };
}

QString NetworkAdapter::backendToNetdevType(NetworkBackend backend)
{
    switch (backend) {
    case NetworkBackend::None: return QString();
    case NetworkBackend::User: return QStringLiteral("user");
    case NetworkBackend::Bridge: return QStringLiteral("bridge");
    case NetworkBackend::Tap: return QStringLiteral("tap");
    case NetworkBackend::Socket: return QStringLiteral("socket");
    default: return QStringLiteral("user");
    }
}

QString NetworkAdapter::nicModelToQemuDevice(NicModel model)
{
    switch (model) {
    case NicModel::Virtio: return QStringLiteral("virtio-net-pci");
    case NicModel::E1000: return QStringLiteral("e1000");
    case NicModel::E1000E: return QStringLiteral("e1000e");
    case NicModel::Rtl8139: return QStringLiteral("rtl8139");
    case NicModel::Pcnet: return QStringLiteral("pcnet");
    case NicModel::I82551: return QStringLiteral("i82551");
    case NicModel::I82557B: return QStringLiteral("i82557b");
    case NicModel::I82559er: return QStringLiteral("i82559er");
    default: return QStringLiteral("virtio-net-pci");
    }
}
