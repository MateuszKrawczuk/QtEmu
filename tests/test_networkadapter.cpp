// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest/QtTest>
#include <QJsonDocument>
#include <QJsonObject>
#include "../src/networkadapter.h"

class TestNetworkAdapter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDefaultValues();
    void testIdGetterSetter();
    void testBackendGetterSetter();
    void testNicModelGetterSetter();
    void testMacAddressGetterSetter();
    void testBridgeNameGetterSetter();
    void testTapDeviceGetterSetter();
    void testSocketGetterSetters();
    void testBootROMGetterSetter();
    void testPortForwards();
    void testToJson();
    void testFromJson();
    void testToNetdevCommandUser();
    void testToNetdevCommandBridge();
    void testToNetdevCommandTap();
    void testToNetdevCommandSocket();
    void testToDeviceCommand();
    void testBackendToStringConversion();
    void testNicModelToStringConversion();

private:
    NetworkAdapter *testAdapter;
};

void TestNetworkAdapter::initTestCase()
{
    qDebug() << "Starting NetworkAdapter tests";
}

void TestNetworkAdapter::cleanupTestCase()
{
    qDebug() << "Finished NetworkAdapter tests";
}

void TestNetworkAdapter::init()
{
    testAdapter = new NetworkAdapter();
}

void TestNetworkAdapter::cleanup()
{
    delete testAdapter;
}

void TestNetworkAdapter::testDefaultValues()
{
    NetworkAdapter *adapter = new NetworkAdapter();
    QVERIFY(adapter->id().isEmpty());
    QCOMPARE(adapter->backend(), NetworkBackend::User);
    QCOMPARE(adapter->nicModel(), NicModel::Virtio);
    QVERIFY(adapter->macAddress().isEmpty());
    QVERIFY(adapter->bridgeName().isEmpty());
    QVERIFY(adapter->tapDevice().isEmpty());
    QVERIFY(adapter->socketListen().isEmpty());
    QVERIFY(adapter->socketConnect().isEmpty());
    QVERIFY(!adapter->bootROM());
    QVERIFY(adapter->portForwards().isEmpty());
    delete adapter;
}

void TestNetworkAdapter::testIdGetterSetter()
{
    testAdapter->setId("net0");
    QCOMPARE(testAdapter->id(), QString("net0"));
}

void TestNetworkAdapter::testBackendGetterSetter()
{
    testAdapter->setBackend(NetworkBackend::User);
    QCOMPARE(testAdapter->backend(), NetworkBackend::User);

    testAdapter->setBackend(NetworkBackend::Bridge);
    QCOMPARE(testAdapter->backend(), NetworkBackend::Bridge);

    testAdapter->setBackend(NetworkBackend::Tap);
    QCOMPARE(testAdapter->backend(), NetworkBackend::Tap);

    testAdapter->setBackend(NetworkBackend::Socket);
    QCOMPARE(testAdapter->backend(), NetworkBackend::Socket);
}

void TestNetworkAdapter::testNicModelGetterSetter()
{
    testAdapter->setNicModel(NicModel::E1000);
    QCOMPARE(testAdapter->nicModel(), NicModel::E1000);

    testAdapter->setNicModel(NicModel::Rtl8139);
    QCOMPARE(testAdapter->nicModel(), NicModel::Rtl8139);

    testAdapter->setNicModel(NicModel::Virtio);
    QCOMPARE(testAdapter->nicModel(), NicModel::Virtio);
}

void TestNetworkAdapter::testMacAddressGetterSetter()
{
    QString mac = "52:54:00:12:34:56";
    testAdapter->setMacAddress(mac);
    QCOMPARE(testAdapter->macAddress(), mac);
}

void TestNetworkAdapter::testBridgeNameGetterSetter()
{
    testAdapter->setBridgeName("br0");
    QCOMPARE(testAdapter->bridgeName(), QString("br0"));
}

void TestNetworkAdapter::testTapDeviceGetterSetter()
{
    testAdapter->setTapDevice("tap0");
    QCOMPARE(testAdapter->tapDevice(), QString("tap0"));
}

void TestNetworkAdapter::testSocketGetterSetters()
{
    testAdapter->setSocketListen("127.0.0.1:1234");
    QCOMPARE(testAdapter->socketListen(), QString("127.0.0.1:1234"));

    testAdapter->setSocketConnect("192.168.1.1:5678");
    QCOMPARE(testAdapter->socketConnect(), QString("192.168.1.1:5678"));
}

void TestNetworkAdapter::testBootROMGetterSetter()
{
    testAdapter->setBootROM(true);
    QVERIFY(testAdapter->bootROM());

    testAdapter->setBootROM(false);
    QVERIFY(!testAdapter->bootROM());
}

void TestNetworkAdapter::testPortForwards()
{
    PortForward pf1;
    pf1.protocol = "tcp";
    pf1.hostPort = 8080;
    pf1.guestPort = 80;
    pf1.hostAddress = "127.0.0.1";
    pf1.guestAddress = "";

    testAdapter->addPortForward(pf1);
    QCOMPARE(testAdapter->portForwards().size(), 1);
    QCOMPARE(testAdapter->portForwards().at(0).protocol, QString("tcp"));
    QCOMPARE(testAdapter->portForwards().at(0).hostPort, 8080);

    PortForward pf2;
    pf2.protocol = "udp";
    pf2.hostPort = 5353;
    pf2.guestPort = 53;

    QList<PortForward> forwards;
    forwards << pf1 << pf2;
    testAdapter->setPortForwards(forwards);
    QCOMPARE(testAdapter->portForwards().size(), 2);

    testAdapter->removePortForward(0);
    QCOMPARE(testAdapter->portForwards().size(), 1);
    QCOMPARE(testAdapter->portForwards().at(0).protocol, QString("udp"));
}

void TestNetworkAdapter::testToJson()
{
    testAdapter->setId("net0");
    testAdapter->setBackend(NetworkBackend::User);
    testAdapter->setNicModel(NicModel::Virtio);
    testAdapter->setMacAddress("52:54:00:12:34:56");

    PortForward pf;
    pf.protocol = "tcp";
    pf.hostPort = 8080;
    pf.guestPort = 80;
    testAdapter->addPortForward(pf);

    QJsonObject json = testAdapter->toJson();

    QCOMPARE(json["id"].toString(), QString("net0"));
    QCOMPARE(json["backend"].toString(), QString("user"));
    QCOMPARE(json["nicModel"].toString(), QString("virtio"));
    QCOMPARE(json["macAddress"].toString(), QString("52:54:00:12:34:56"));

    QJsonArray portForwards = json["portForwards"].toArray();
    QCOMPARE(portForwards.size(), 1);
}

void TestNetworkAdapter::testFromJson()
{
    QJsonObject json;
    json["id"] = "net1";
    json["backend"] = "bridge";
    json["nicModel"] = "e1000";
    json["macAddress"] = "52:54:00:AB:CD:EF";
    json["bridgeName"] = "br0";
    json["bootROM"] = true;

    QJsonArray portForwards;
    QJsonObject pf;
    pf["protocol"] = "tcp";
    pf["hostPort"] = 443;
    pf["guestPort"] = 443;
    pf["hostAddress"] = "0.0.0.0";
    pf["guestAddress"] = "";
    portForwards.append(pf);
    json["portForwards"] = portForwards;

    NetworkAdapter *adapter = NetworkAdapter::fromJson(json);
    QVERIFY(adapter != nullptr);
    QCOMPARE(adapter->id(), QString("net1"));
    QCOMPARE(adapter->backend(), NetworkBackend::Bridge);
    QCOMPARE(adapter->nicModel(), NicModel::E1000);
    QCOMPARE(adapter->macAddress(), QString("52:54:00:AB:CD:EF"));
    QCOMPARE(adapter->bridgeName(), QString("br0"));
    QVERIFY(adapter->bootROM());
    QCOMPARE(adapter->portForwards().size(), 1);

    delete adapter;
}

void TestNetworkAdapter::testToNetdevCommandUser()
{
    testAdapter->setId("net0");
    testAdapter->setBackend(NetworkBackend::User);

    PortForward pf;
    pf.protocol = "tcp";
    pf.hostPort = 8080;
    pf.guestPort = 80;
    pf.hostAddress = "";
    pf.guestAddress = "";
    testAdapter->addPortForward(pf);

    QString cmd = testAdapter->toNetdevCommand();
    QVERIFY(cmd.contains("user"));
    QVERIFY(cmd.contains("id=net0"));
    QVERIFY(cmd.contains("hostfwd=tcp:8080-80"));
}

void TestNetworkAdapter::testToNetdevCommandBridge()
{
    testAdapter->setId("net0");
    testAdapter->setBackend(NetworkBackend::Bridge);
    testAdapter->setBridgeName("br0");

    QString cmd = testAdapter->toNetdevCommand();
    QVERIFY(cmd.contains("bridge"));
    QVERIFY(cmd.contains("id=net0"));
    QVERIFY(cmd.contains("br=br0"));
}

void TestNetworkAdapter::testToNetdevCommandTap()
{
    testAdapter->setId("net0");
    testAdapter->setBackend(NetworkBackend::Tap);
    testAdapter->setTapDevice("tap0");

    QString cmd = testAdapter->toNetdevCommand();
    QVERIFY(cmd.contains("tap"));
    QVERIFY(cmd.contains("id=net0"));
    QVERIFY(cmd.contains("ifname=tap0"));
}

void TestNetworkAdapter::testToNetdevCommandSocket()
{
    testAdapter->setId("net0");
    testAdapter->setBackend(NetworkBackend::Socket);
    testAdapter->setSocketListen("127.0.0.1:1234");

    QString cmd = testAdapter->toNetdevCommand();
    QVERIFY(cmd.contains("socket"));
    QVERIFY(cmd.contains("id=net0"));
    QVERIFY(cmd.contains("listen"));
}

void TestNetworkAdapter::testToDeviceCommand()
{
    testAdapter->setId("net0");
    testAdapter->setNicModel(NicModel::Virtio);
    testAdapter->setMacAddress("52:54:00:12:34:56");

    QString cmd = testAdapter->toDeviceCommand();
    QVERIFY(cmd.contains("virtio-net-pci"));
    QVERIFY(cmd.contains("netdev=net0"));
    QVERIFY(cmd.contains("mac=52:54:00:12:34:56"));
}

void TestNetworkAdapter::testBackendToStringConversion()
{
    QCOMPARE(NetworkAdapter::backendToString(NetworkBackend::None), QString("none"));
    QCOMPARE(NetworkAdapter::backendToString(NetworkBackend::User), QString("user"));
    QCOMPARE(NetworkAdapter::backendToString(NetworkBackend::Bridge), QString("bridge"));
    QCOMPARE(NetworkAdapter::backendToString(NetworkBackend::Tap), QString("tap"));
    QCOMPARE(NetworkAdapter::backendToString(NetworkBackend::Socket), QString("socket"));

    QCOMPARE(NetworkAdapter::stringToBackend("none"), NetworkBackend::None);
    QCOMPARE(NetworkAdapter::stringToBackend("user"), NetworkBackend::User);
    QCOMPARE(NetworkAdapter::stringToBackend("bridge"), NetworkBackend::Bridge);
    QCOMPARE(NetworkAdapter::stringToBackend("tap"), NetworkBackend::Tap);
    QCOMPARE(NetworkAdapter::stringToBackend("socket"), NetworkBackend::Socket);
}

void TestNetworkAdapter::testNicModelToStringConversion()
{
    QCOMPARE(NetworkAdapter::nicModelToString(NicModel::Virtio), QString("virtio"));
    QCOMPARE(NetworkAdapter::nicModelToString(NicModel::E1000), QString("e1000"));
    QCOMPARE(NetworkAdapter::nicModelToString(NicModel::E1000E), QString("e1000e"));
    QCOMPARE(NetworkAdapter::nicModelToString(NicModel::Rtl8139), QString("rtl8139"));

    QCOMPARE(NetworkAdapter::stringToNicModel("virtio"), NicModel::Virtio);
    QCOMPARE(NetworkAdapter::stringToNicModel("e1000"), NicModel::E1000);
    QCOMPARE(NetworkAdapter::stringToNicModel("e1000e"), NicModel::E1000E);
    QCOMPARE(NetworkAdapter::stringToNicModel("rtl8139"), NicModel::Rtl8139);
}

QTEST_MAIN(TestNetworkAdapter)
#include "test_networkadapter.moc"
