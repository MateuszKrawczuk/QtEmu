// SPDX-FileCopyrightText: 2026 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest/QtTest>
#include "../src/utils/networkutils.h"
#include "../src/networkadapter.h"

class TestNetworkUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testGenerateMacAddress();
    void testGenerateMacAddressWithCustomPrefix();
    void testIsValidMacAddress();
    void testIsValidMacAddressInvalid();
    void testGetNicModelNames();
    void testNicModelToQemu();
    void testIsBridgeSupported();
    void testIsTapSupported();
};

void TestNetworkUtils::initTestCase()
{
    qDebug() << "Starting NetworkUtils tests";
}

void TestNetworkUtils::cleanupTestCase()
{
    qDebug() << "Finished NetworkUtils tests";
}

void TestNetworkUtils::testGenerateMacAddress()
{
    QString mac = NetworkUtils::generateMacAddress();
    
    QVERIFY(!mac.isEmpty());
    QVERIFY(NetworkUtils::isValidMacAddress(mac));
    QVERIFY(mac.startsWith("52:54:00"));
}

void TestNetworkUtils::testGenerateMacAddressWithCustomPrefix()
{
    QString customPrefix = "52:54:01";
    QString mac = NetworkUtils::generateMacAddress(customPrefix);
    
    QVERIFY(!mac.isEmpty());
    QVERIFY(NetworkUtils::isValidMacAddress(mac));
    QVERIFY(mac.startsWith(customPrefix));
}

void TestNetworkUtils::testIsValidMacAddress()
{
    QVERIFY(NetworkUtils::isValidMacAddress("52:54:00:12:34:56"));
    QVERIFY(NetworkUtils::isValidMacAddress("00:11:22:33:44:55"));
    QVERIFY(NetworkUtils::isValidMacAddress("AA:BB:CC:DD:EE:FF"));
    QVERIFY(NetworkUtils::isValidMacAddress("aa:bb:cc:dd:ee:ff"));
}

void TestNetworkUtils::testIsValidMacAddressInvalid()
{
    QVERIFY(!NetworkUtils::isValidMacAddress(""));
    QVERIFY(!NetworkUtils::isValidMacAddress("invalid"));
    QVERIFY(!NetworkUtils::isValidMacAddress("52:54:00:12:34"));
    QVERIFY(!NetworkUtils::isValidMacAddress("52:54:00:12:34:56:78"));
    QVERIFY(!NetworkUtils::isValidMacAddress("52-54-00-12-34-56"));
    QVERIFY(!NetworkUtils::isValidMacAddress("52:54:00:12:34:5G"));
}

void TestNetworkUtils::testGetNicModelNames()
{
    QStringList models = NetworkUtils::getNicModelNames();
    
    QVERIFY(!models.isEmpty());
    QVERIFY(models.contains("virtio"));
    QVERIFY(models.contains("e1000"));
    QVERIFY(models.contains("rtl8139"));
}

void TestNetworkUtils::testNicModelToQemu()
{
    QCOMPARE(NetworkUtils::nicModelToQemu(NicModel::Virtio), QString("virtio"));
    QCOMPARE(NetworkUtils::nicModelToQemu(NicModel::E1000), QString("e1000"));
    QCOMPARE(NetworkUtils::nicModelToQemu(NicModel::E1000E), QString("e1000e"));
    QCOMPARE(NetworkUtils::nicModelToQemu(NicModel::Rtl8139), QString("rtl8139"));
    QCOMPARE(NetworkUtils::nicModelToQemu(NicModel::Pcnet), QString("pcnet"));
}

void TestNetworkUtils::testIsBridgeSupported()
{
    bool supported = NetworkUtils::isBridgeSupported();
#ifdef Q_OS_LINUX
    QVERIFY(supported);
#else
    Q_UNUSED(supported);
#endif
}

void TestNetworkUtils::testIsTapSupported()
{
    bool supported = NetworkUtils::isTapSupported();
#ifdef Q_OS_LINUX
    QVERIFY(supported);
#else
    Q_UNUSED(supported);
#endif
}

QTEST_MAIN(TestNetworkUtils)
#include "test_networkutils.moc"
