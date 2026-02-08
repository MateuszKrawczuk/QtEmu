// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "../src/machine.h"
#include "../src/machineutils.h"

class TestMachineCloudInit : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases
    void testCloudInitEnabledGetterSetter();
    void testCloudInitHostnameGetterSetter();
    void testCloudInitUsernameGetterSetter();
    void testCloudInitPasswordGetterSetter();
    void testCloudInitSSHKeyGetterSetter();
    void testCloudInitUserDataGetterSetter();
    void testCloudInitDefaultValues();
    void testCloudInitJSONSerialization();
    void testCloudInitJSONDeserialization();
    void testPasswordNotSavedToJSON();

private:
    Machine *testMachine;
    QTemporaryDir *tempDir;
};

void TestMachineCloudInit::initTestCase()
{
    qDebug() << "Starting Machine CloudInit property tests";
}

void TestMachineCloudInit::cleanupTestCase()
{
    qDebug() << "Finished Machine CloudInit property tests";
}

void TestMachineCloudInit::init()
{
    tempDir = new QTemporaryDir();
    QVERIFY(tempDir->isValid());

    testMachine = new Machine();
    testMachine->setName("TestVM");
    testMachine->setPath(tempDir->path());
    testMachine->setUuid(QUuid::createUuid());
    testMachine->setConfigPath(tempDir->path() + "/machine.json");
}

void TestMachineCloudInit::cleanup()
{
    delete testMachine;
    delete tempDir;
}

void TestMachineCloudInit::testCloudInitEnabledGetterSetter()
{
    testMachine->setCloudInitEnabled(true);
    QVERIFY(testMachine->getCloudInitEnabled());

    testMachine->setCloudInitEnabled(false);
    QVERIFY(!testMachine->getCloudInitEnabled());
}

void TestMachineCloudInit::testCloudInitHostnameGetterSetter()
{
    QString hostname = "test-hostname";
    testMachine->setCloudInitHostname(hostname);
    QCOMPARE(testMachine->getCloudInitHostname(), hostname);
}

void TestMachineCloudInit::testCloudInitUsernameGetterSetter()
{
    QString username = "testuser";
    testMachine->setCloudInitUsername(username);
    QCOMPARE(testMachine->getCloudInitUsername(), username);
}

void TestMachineCloudInit::testCloudInitPasswordGetterSetter()
{
    QString password = "testPassword123";
    testMachine->setCloudInitPassword(password);
    QCOMPARE(testMachine->getCloudInitPassword(), password);
}

void TestMachineCloudInit::testCloudInitSSHKeyGetterSetter()
{
    QString sshKey = "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC... test@example.com";
    testMachine->setCloudInitSSHKey(sshKey);
    QCOMPARE(testMachine->getCloudInitSSHKey(), sshKey);
}

void TestMachineCloudInit::testCloudInitUserDataGetterSetter()
{
    QString userData = "#cloud-config\npackages:\n  - vim";
    testMachine->setCloudInitUserData(userData);
    QCOMPARE(testMachine->getCloudInitUserData(), userData);
}

void TestMachineCloudInit::testCloudInitDefaultValues()
{
    Machine *newMachine = new Machine();

    QVERIFY(!newMachine->getCloudInitEnabled());
    QVERIFY(newMachine->getCloudInitHostname().isEmpty());
    QCOMPARE(newMachine->getCloudInitUsername(), QString("ubuntu"));
    QVERIFY(newMachine->getCloudInitPassword().isEmpty());
    QVERIFY(newMachine->getCloudInitSSHKey().isEmpty());
    QVERIFY(newMachine->getCloudInitUserData().isEmpty());

    delete newMachine;
}

void TestMachineCloudInit::testCloudInitJSONSerialization()
{
    testMachine->setCloudInitEnabled(true);
    testMachine->setCloudInitHostname("test-host");
    testMachine->setCloudInitUsername("testuser");
    testMachine->setCloudInitPassword("testpass");
    testMachine->setCloudInitSSHKey("ssh-rsa test-key");
    testMachine->setCloudInitUserData("#cloud-config\ntest");

    // Save machine
    bool saved = testMachine->saveMachine();
    QVERIFY(saved);

    // Read JSON file
    QFile file(testMachine->getConfigPath());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();

    QVERIFY(json.contains("cloudInit"));
    QJsonObject cloudInit = json["cloudInit"].toObject();

    QCOMPARE(cloudInit["enabled"].toBool(), true);
    QCOMPARE(cloudInit["hostname"].toString(), QString("test-host"));
    QCOMPARE(cloudInit["username"].toString(), QString("testuser"));
    QCOMPARE(cloudInit["sshKey"].toString(), QString("ssh-rsa test-key"));
    QCOMPARE(cloudInit["userData"].toString(), QString("#cloud-config\ntest"));
}

void TestMachineCloudInit::testCloudInitJSONDeserialization()
{
    // Create JSON file
    QJsonObject cloudInit;
    cloudInit["enabled"] = true;
    cloudInit["hostname"] = "loaded-host";
    cloudInit["username"] = "loadeduser";
    cloudInit["sshKey"] = "ssh-rsa loaded-key";
    cloudInit["userData"] = "#cloud-config\nloaded";

    QJsonObject machineJson;
    machineJson["name"] = "TestMachine";
    machineJson["cloudInit"] = cloudInit;
    machineJson["uuid"] = testMachine->getUuid().toString();
    machineJson["path"] = tempDir->path();

    // Add required fields
    machineJson["OSType"] = "Linux";
    machineJson["OSVersion"] = "Ubuntu";
    machineJson["type"] = "pc";
    machineJson["RAM"] = 1024;
    machineJson["network"] = true;
    machineJson["hostsoundsystem"] = "alsa";

    QJsonObject cpu;
    cpu["CPUType"] = "host";
    cpu["CPUCount"] = 1;
    cpu["socketCount"] = 1;
    cpu["coresSocket"] = 1;
    cpu["threadsCore"] = 1;
    cpu["maxHotCPU"] = 1;
    machineJson["cpu"] = cpu;

    QJsonObject gpu;
    gpu["GPUType"] = "std";
    gpu["keyboard"] = "en-us";
    machineJson["gpu"] = gpu;

    QJsonObject boot;
    boot["bootMenu"] = true;
    QJsonObject kernelBoot;
    kernelBoot["enabled"] = false;
    kernelBoot["kernelPath"] = "";
    kernelBoot["initrdPath"] = "";
    kernelBoot["kernelArgs"] = "";
    boot["kernelBoot"] = kernelBoot;
    boot["bootOrder"] = QJsonArray::fromStringList(QStringList() << "c");
    machineJson["boot"] = boot;

    machineJson["media"] = QJsonArray();
    machineJson["accelerator"] = QJsonArray::fromStringList(QStringList() << "tcg");
    machineJson["audio"] = QJsonArray();

    QJsonDocument doc(machineJson);
    QFile file(testMachine->getConfigPath());
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(doc.toJson());
    file.close();

    // Load machine
    Machine *loadedMachine = new Machine();
    QJsonObject loadedJson = MachineUtils::readMachineFile(testMachine->getConfigPath());
    MachineUtils::fillMachineObject(loadedMachine, loadedJson, testMachine->getConfigPath());

    QVERIFY(loadedMachine->getCloudInitEnabled());
    QCOMPARE(loadedMachine->getCloudInitHostname(), QString("loaded-host"));
    QCOMPARE(loadedMachine->getCloudInitUsername(), QString("loadeduser"));
    QCOMPARE(loadedMachine->getCloudInitSSHKey(), QString("ssh-rsa loaded-key"));
    QCOMPARE(loadedMachine->getCloudInitUserData(), QString("#cloud-config\nloaded"));

    delete loadedMachine;
}

void TestMachineCloudInit::testPasswordNotSavedToJSON()
{
    testMachine->setCloudInitEnabled(true);
    testMachine->setCloudInitPassword("supersecret");

    // Save machine
    bool saved = testMachine->saveMachine();
    QVERIFY(saved);

    // Read JSON file
    QFile file(testMachine->getConfigPath());
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray data = file.readAll();
    file.close();

    QString jsonContent = QString::fromUtf8(data);

    // Password should NOT appear in JSON
    QVERIFY(!jsonContent.contains("supersecret"));
    QVERIFY(!jsonContent.contains("password"));

    // cloudInit section should exist but without password field
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject json = doc.object();
    QJsonObject cloudInit = json["cloudInit"].toObject();

    QVERIFY(!cloudInit.contains("password"));
}

QTEST_MAIN(TestMachineCloudInit)
#include "test_machine_cloudinit.moc"
