// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include "../src/machine.h"
#include "../src/machineutils.h"

class TestMachineCustomArgs : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Test cases
    void testCustomArgumentsGetterSetter();
    void testCustomArgumentsDefaultValue();
    void testCustomArgumentsEmptyString();
    void testCustomArgumentsWhitespace();
    void testCustomArgumentsJSONSerialization();
    void testCustomArgumentsJSONDeserialization();
    void testCustomArgumentsWithSpecialCharacters();
    void testCustomArgumentsWithQuotes();
    void testCustomArgumentsSplitCommand();

private:
    Machine *testMachine;
    QTemporaryDir *tempDir;
};

void TestMachineCustomArgs::initTestCase()
{
    qDebug() << "Starting Machine CustomArguments property tests";
}

void TestMachineCustomArgs::cleanupTestCase()
{
    qDebug() << "Finished Machine CustomArguments property tests";
}

void TestMachineCustomArgs::init()
{
    tempDir = new QTemporaryDir();
    QVERIFY(tempDir->isValid());

    testMachine = new Machine();
    testMachine->setName("TestVM");
    testMachine->setPath(tempDir->path());
    testMachine->setUuid(QUuid::createUuid());
    testMachine->setConfigPath(tempDir->path() + "/machine.json");
}

void TestMachineCustomArgs::cleanup()
{
    delete testMachine;
    delete tempDir;
}

void TestMachineCustomArgs::testCustomArgumentsGetterSetter()
{
    QString args = "-device virtio-balloon -global kvm-irqchip.off=true";
    testMachine->setCustomArguments(args);
    QCOMPARE(testMachine->getCustomArguments(), args);
}

void TestMachineCustomArgs::testCustomArgumentsDefaultValue()
{
    Machine *newMachine = new Machine();
    QVERIFY(newMachine->getCustomArguments().isEmpty());
    delete newMachine;
}

void TestMachineCustomArgs::testCustomArgumentsEmptyString()
{
    testMachine->setCustomArguments("");
    QVERIFY(testMachine->getCustomArguments().isEmpty());

    testMachine->setCustomArguments("-device test");
    testMachine->setCustomArguments("");
    QVERIFY(testMachine->getCustomArguments().isEmpty());
}

void TestMachineCustomArgs::testCustomArgumentsWhitespace()
{
    testMachine->setCustomArguments("   ");
    QCOMPARE(testMachine->getCustomArguments(), QString("   "));

    // Note: The UI should trim whitespace, but the setter stores as-is
    testMachine->setCustomArguments("  -device test  ");
    QCOMPARE(testMachine->getCustomArguments(), QString("  -device test  "));
}

void TestMachineCustomArgs::testCustomArgumentsJSONSerialization()
{
    QString args = "-device virtio-net-pci,netdev=net0 -netdev user,id=net0";
    testMachine->setCustomArguments(args);

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

    QVERIFY(json.contains("customArguments"));
    QCOMPARE(json["customArguments"].toString(), args);
}

void TestMachineCustomArgs::testCustomArgumentsJSONDeserialization()
{
    QString args = "-device usb-tablet -device usb-kbd";

    // Create JSON file
    QJsonObject machineJson;
    machineJson["name"] = "TestMachine";
    machineJson["customArguments"] = args;
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

    QCOMPARE(loadedMachine->getCustomArguments(), args);

    delete loadedMachine;
}

void TestMachineCustomArgs::testCustomArgumentsWithSpecialCharacters()
{
    // Test with special characters that might appear in QEMU arguments
    QString args = "-device virtio-serial-pci -device virtserialport,chardev=spicechannel0,name=com.redhat.spice.0";
    testMachine->setCustomArguments(args);
    QCOMPARE(testMachine->getCustomArguments(), args);

    // Save and reload to ensure special characters survive JSON round-trip
    testMachine->saveMachine();

    Machine *loadedMachine = new Machine();
    QJsonObject loadedJson = MachineUtils::readMachineFile(testMachine->getConfigPath());
    MachineUtils::fillMachineObject(loadedMachine, loadedJson, testMachine->getConfigPath());

    QCOMPARE(loadedMachine->getCustomArguments(), args);

    delete loadedMachine;
}

void TestMachineCustomArgs::testCustomArgumentsWithQuotes()
{
    // Test with quoted arguments (common in QEMU command lines)
    QString args = "-device usb-host,vendorid=0x1234,productid=0x5678 -name \"My Virtual Machine\"";
    testMachine->setCustomArguments(args);
    QCOMPARE(testMachine->getCustomArguments(), args);
}

void TestMachineCustomArgs::testCustomArgumentsSplitCommand()
{
    // Test that QProcess::splitCommand correctly parses our custom arguments
    QString args = "-device virtio-balloon -m 2048 -name \"Test VM\"";

    QStringList splitArgs = QProcess::splitCommand(args);

    QCOMPARE(splitArgs.size(), 6);
    QCOMPARE(splitArgs[0], QString("-device"));
    QCOMPARE(splitArgs[1], QString("virtio-balloon"));
    QCOMPARE(splitArgs[2], QString("-m"));
    QCOMPARE(splitArgs[3], QString("2048"));
    QCOMPARE(splitArgs[4], QString("-name"));
    QCOMPARE(splitArgs[5], QString("Test VM")); // Quotes should be stripped

    // Test empty string
    QStringList emptyArgs = QProcess::splitCommand(QStringLiteral(""));
    QVERIFY(emptyArgs.isEmpty());

    // Test whitespace only
    QStringList whitespaceArgs = QProcess::splitCommand(QStringLiteral("   "));
    QVERIFY(whitespaceArgs.isEmpty());
}

QTEST_MAIN(TestMachineCustomArgs)
#include "test_machine_customargs.moc"
