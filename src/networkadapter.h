// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef NETWORKADAPTER_H
#define NETWORKADAPTER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QList>

enum class NetworkBackend {
    None = 0,
    User = 1,
    Bridge = 2,
    Tap = 3,
    Socket = 4
};

enum class NicModel {
    Virtio = 0,
    E1000 = 1,
    E1000E = 2,
    Rtl8139 = 3,
    Pcnet = 4,
    I82551 = 5,
    I82557B = 6,
    I82559er = 7
};

struct PortForward {
    QString protocol;
    int hostPort;
    int guestPort;
    QString hostAddress;
    QString guestAddress;
};

class NetworkAdapter : public QObject {
    Q_OBJECT

public:
    explicit NetworkAdapter(QObject *parent = nullptr);
    ~NetworkAdapter();

    QString id() const;
    void setId(const QString &id);

    NetworkBackend backend() const;
    void setBackend(NetworkBackend backend);

    NicModel nicModel() const;
    void setNicModel(NicModel model);

    QString macAddress() const;
    void setMacAddress(const QString &mac);

    QString bridgeName() const;
    void setBridgeName(const QString &name);

    QString tapDevice() const;
    void setTapDevice(const QString &device);

    QString socketListen() const;
    void setSocketListen(const QString &address);

    QString socketConnect() const;
    void setSocketConnect(const QString &address);

    bool bootROM() const;
    void setBootROM(bool enabled);

    QList<PortForward> portForwards() const;
    void setPortForwards(const QList<PortForward> &forwards);
    void addPortForward(const PortForward &forward);
    void removePortForward(int index);

    QJsonObject toJson() const;
    static NetworkAdapter* fromJson(const QJsonObject &json, QObject *parent = nullptr);

    QString toNetdevCommand() const;
    QString toDeviceCommand() const;

    static QString backendToString(NetworkBackend backend);
    static NetworkBackend stringToBackend(const QString &str);
    static QString nicModelToString(NicModel model);
    static NicModel stringToNicModel(const QString &str);
    static QStringList availableNicModelNames();

private:
    static QString backendToNetdevType(NetworkBackend backend);
    static QString nicModelToQemuDevice(NicModel model);

private:
    QString m_id;
    NetworkBackend m_backend;
    NicModel m_nicModel;
    QString m_macAddress;
    QString m_bridgeName;
    QString m_tapDevice;
    QString m_socketListen;
    QString m_socketConnect;
    bool m_bootROM;
    QList<PortForward> m_portForwards;
};

#endif
