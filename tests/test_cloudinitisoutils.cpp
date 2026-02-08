// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QFile>
#include "../src/utils/cloudinitisoutils.h"
#include "../src/machine.h"
#include "../src/qemu.h"

class TestCloudInitIsoUtils : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases
    void testGenerateUserDataBasic();
    void testGenerateUserDataWithPassword();
    void testGenerateUserDataWithSSHKey();
    void testGenerateUserDataWithCustomData();
    void testGenerateMetaData();
    void testPasswordHashing();
    void testPasswordHashingNotEmpty();
    void testPasswordHashingValidFormat();
    void testISOGeneration();
    void testISOCleanup();
    void testYAMLValidation();

private:
    Machine *testMachine;
    QTemporaryDir *tempDir;
    QEMU *qemuObj;
};

void TestCloudInitIsoUtils::initTestCase()
{
    qDebug() << "Starting CloudInitIsoUtils tests";
}

void TestCloudInitIsoUtils::cleanupTestCase()
{
    qDebug() << "Finished CloudInitIsoUtils tests";
}

void TestCloudInitIsoUtils::init()
{
    // Create temporary directory for test machine
    tempDir = new QTemporaryDir();
    QVERIFY(tempDir->isValid());

    // Create test machine
    qemuObj = new QEMU();
    testMachine = new Machine();
    testMachine->setName("TestVM");
    testMachine->setPath(tempDir->path());
    testMachine->setUuid(QUuid::createUuid());

    // Set default cloud-init values
    testMachine->setCloudInitEnabled(true);
    testMachine->setCloudInitHostname("test-host");
    testMachine->setCloudInitUsername("testuser");
}

void TestCloudInitIsoUtils::cleanup()
{
    delete testMachine;
    delete qemuObj;
    delete tempDir;
}

void TestCloudInitIsoUtils::testGenerateUserDataBasic()
{
    testMachine->setCloudInitPassword("");
    testMachine->setCloudInitSSHKey("");
    testMachine->setCloudInitUserData("");

    QString userData = CloudInitIsoUtils::generateUserData(testMachine);

    QVERIFY(!userData.isEmpty());
    QVERIFY(userData.startsWith("#cloud-config"));
    QVERIFY(userData.contains("hostname: test-host"));
    QVERIFY(userData.contains("name: testuser"));
}

void TestCloudInitIsoUtils::testGenerateUserDataWithPassword()
{
    testMachine->setCloudInitPassword("testpass123");

    QString userData = CloudInitIsoUtils::generateUserData(testMachine);

    QVERIFY(userData.contains("passwd:"));
    QVERIFY(userData.contains("$6$")); // SHA-512 hash indicator
    QVERIFY(!userData.contains("testpass123")); // Plain password should not appear
}

void TestCloudInitIsoUtils::testGenerateUserDataWithSSHKey()
{
    QString testKey = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC... test@example.com";
    testMachine->setCloudInitSSHKey(testKey);

    QString userData = CloudInitIsoUtils::generateUserData(testMachine);

    QVERIFY(userData.contains("ssh_authorized_keys:"));
    QVERIFY(userData.contains(testKey));
}

void TestCloudInitIsoUtils::testGenerateUserDataWithCustomData()
{
    QString customData = "#cloud-config\npackages:\n  - vim\n  - htop";
    testMachine->setCloudInitUserData(customData);

    QString userData = CloudInitIsoUtils::generateUserData(testMachine);

    QCOMPARE(userData, customData);
}

void TestCloudInitIsoUtils::testGenerateMetaData()
{
    QString metaData = CloudInitIsoUtils::generateMetaData(testMachine);

    QVERIFY(!metaData.isEmpty());
    QVERIFY(metaData.contains("instance-id:"));
    QVERIFY(metaData.contains(testMachine->getUuid().toString(QUuid::WithoutBraces)));
    QVERIFY(metaData.contains("local-hostname:"));
}

void TestCloudInitIsoUtils::testPasswordHashing()
{
    QString password = "testPassword123!";
    QString hash1 = CloudInitIsoUtils::hashPassword(password);
    QString hash2 = CloudInitIsoUtils::hashPassword(password);

    // Hashes should be different due to random salt
    QVERIFY(hash1 != hash2);
}

void TestCloudInitIsoUtils::testPasswordHashingNotEmpty()
{
    QString password = "test";
    QString hash = CloudInitIsoUtils::hashPassword(password);

    QVERIFY(!hash.isEmpty());
}

void TestCloudInitIsoUtils::testPasswordHashingValidFormat()
{
    QString password = "test123";
    QString hash = CloudInitIsoUtils::hashPassword(password);

    // Should start with $6$ (SHA-512 indicator)
    QVERIFY(hash.startsWith("$6$"));

    // Should contain multiple $ separators
    QVERIFY(hash.count('$') >= 3);
}

void TestCloudInitIsoUtils::testISOGeneration()
{
    testMachine->setCloudInitEnabled(true);
    testMachine->setCloudInitHostname("test-vm");
    testMachine->setCloudInitUsername("ubuntu");

    QString isoPath = CloudInitIsoUtils::generateCloudInitISO(testMachine);

    // ISO generation might fail if tools are not available, so we check for that
    if (!isoPath.isEmpty()) {
        QVERIFY(QFile::exists(isoPath));
        QVERIFY(isoPath.contains("qtemu-cloud-init"));
        QVERIFY(isoPath.endsWith(".iso"));

        // Clean up
        QFile::remove(isoPath);
    } else {
        qWarning() << "ISO generation skipped - no ISO creation tools available";
        QSKIP("ISO creation tools not available");
    }
}

void TestCloudInitIsoUtils::testISOCleanup()
{
    QString testIsoPath = tempDir->path() + "/test.iso";

    // Create a dummy file
    QFile file(testIsoPath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("test");
    file.close();

    QVERIFY(QFile::exists(testIsoPath));

    bool cleaned = CloudInitIsoUtils::cleanupCloudInitISO(testIsoPath);
    QVERIFY(cleaned);
    QVERIFY(!QFile::exists(testIsoPath));
}

void TestCloudInitIsoUtils::testYAMLValidation()
{
    // Test with invalid YAML (missing #cloud-config header)
    testMachine->setCloudInitUserData("packages:\n  - vim");

    QString isoPath = CloudInitIsoUtils::generateCloudInitISO(testMachine);

    // Should return empty string due to validation failure
    QVERIFY(isoPath.isEmpty());
}

QTEST_MAIN(TestCloudInitIsoUtils)
#include "test_cloudinitisoutils.moc"
