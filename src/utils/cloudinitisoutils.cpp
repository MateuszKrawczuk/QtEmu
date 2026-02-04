// SPDX-FileCopyrightText: 2025 Mateusz Krawczuk <mateusz.krawczuk@cybrixsystems.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// Local
#include "cloudinitisoutils.h"

CloudInitIsoUtils::CloudInitIsoUtils(QObject *parent) : QObject(parent)
{
    qDebug() << "CloudInitIsoUtils object created";
}

CloudInitIsoUtils::~CloudInitIsoUtils()
{
    qDebug() << "CloudInitIsoUtils object destroyed";
}

/**
 * @brief Generate cloud-init ISO for a machine
 * @param machine, machine to generate cloud-init ISO for
 * @return path to the generated ISO file, or empty string on failure
 *
 * Generate a cloud-init seed ISO with user-data and meta-data
 */
QString CloudInitIsoUtils::generateCloudInitISO(Machine *machine)
{
    if (!machine->getCloudInitEnabled()) {
        return QString();
    }

    // Create a temporary directory for cloud-init files
    QString tempDirPath = machine->getPath() + QDir::separator() + "cloud-init-temp";
    QDir tempDir(tempDirPath);

    // Remove old temp directory if it exists
    if (tempDir.exists()) {
        tempDir.removeRecursively();
    }

    if (!tempDir.mkpath(tempDirPath)) {
        qWarning() << "Failed to create cloud-init temp directory:" << tempDirPath;
        return QString();
    }

    // Generate user-data file
    QString userData = generateUserData(machine);
    QString userDataPath = tempDirPath + QDir::separator() + "user-data";
    QFile userDataFile(userDataPath);
    if (!userDataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to create user-data file:" << userDataPath;
        tempDir.removeRecursively();
        return QString();
    }
    userDataFile.write(userData.toUtf8());
    userDataFile.close();

    // Generate meta-data file
    QString metaData = generateMetaData(machine);
    QString metaDataPath = tempDirPath + QDir::separator() + "meta-data";
    QFile metaDataFile(metaDataPath);
    if (!metaDataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to create meta-data file:" << metaDataPath;
        tempDir.removeRecursively();
        return QString();
    }
    metaDataFile.write(metaData.toUtf8());
    metaDataFile.close();

    // Create ISO file
    QString isoPath = machine->getPath() + QDir::separator() + "cloud-init.iso";

    // Remove old ISO if it exists
    if (QFile::exists(isoPath)) {
        QFile::remove(isoPath);
    }

    if (!createISO(tempDirPath, isoPath)) {
        qWarning() << "Failed to create cloud-init ISO";
        tempDir.removeRecursively();
        return QString();
    }

    // Clean up temp directory
    tempDir.removeRecursively();

    qDebug() << "Cloud-init ISO generated:" << isoPath;
    return isoPath;
}

/**
 * @brief Clean up cloud-init ISO file
 * @param isoPath, path to the ISO file to clean up
 * @return true if cleanup was successful
 *
 * Remove the cloud-init ISO file
 */
bool CloudInitIsoUtils::cleanupCloudInitISO(const QString &isoPath)
{
    if (isoPath.isEmpty() || !QFile::exists(isoPath)) {
        return true;
    }

    return QFile::remove(isoPath);
}

/**
 * @brief Generate user-data content
 * @param machine, machine to generate user-data for
 * @return user-data YAML content
 *
 * Generate cloud-init user-data YAML configuration
 */
QString CloudInitIsoUtils::generateUserData(Machine *machine)
{
    QString userData;

    // If custom user-data is provided, use it
    if (!machine->getCloudInitUserData().isEmpty()) {
        userData = machine->getCloudInitUserData();
        // Ensure it starts with #cloud-config
        if (!userData.startsWith("#cloud-config")) {
            userData = "#cloud-config\n" + userData;
        }
        return userData;
    }

    // Generate basic user-data
    userData = "#cloud-config\n";

    // Set hostname
    if (!machine->getCloudInitHostname().isEmpty()) {
        userData += "hostname: " + machine->getCloudInitHostname() + "\n";
        userData += "fqdn: " + machine->getCloudInitHostname() + "\n";
    }

    // Configure user
    QString username = machine->getCloudInitUsername();
    if (username.isEmpty()) {
        username = "ubuntu";
    }

    userData += "users:\n";
    userData += "  - name: " + username + "\n";
    userData += "    sudo: ALL=(ALL) NOPASSWD:ALL\n";
    userData += "    shell: /bin/bash\n";
    userData += "    groups: users, admin\n";

    // Add password if provided
    if (!machine->getCloudInitPassword().isEmpty()) {
        // Note: In production, this should be hashed
        // For simplicity, we're using plain text here
        // Cloud-init will hash it automatically
        userData += "    plain_text_passwd: " + machine->getCloudInitPassword() + "\n";
        userData += "    lock_passwd: false\n";
    }

    // Add SSH key if provided
    if (!machine->getCloudInitSSHKey().isEmpty()) {
        userData += "    ssh_authorized_keys:\n";
        userData += "      - " + machine->getCloudInitSSHKey() + "\n";
    }

    // Enable password authentication for SSH if password is set
    if (!machine->getCloudInitPassword().isEmpty()) {
        userData += "ssh_pwauth: true\n";
    }

    // Disable cloud-init after first boot
    userData += "package_update: true\n";
    userData += "package_upgrade: false\n";

    return userData;
}

/**
 * @brief Generate meta-data content
 * @param machine, machine to generate meta-data for
 * @return meta-data YAML content
 *
 * Generate cloud-init meta-data YAML configuration
 */
QString CloudInitIsoUtils::generateMetaData(Machine *machine)
{
    QString metaData;

    // Instance ID (use machine UUID)
    metaData += "instance-id: " + machine->getUuid().toString(QUuid::WithoutBraces) + "\n";

    // Local hostname
    QString hostname = machine->getCloudInitHostname();
    if (hostname.isEmpty()) {
        hostname = machine->getName().replace(" ", "-").toLower();
    }
    metaData += "local-hostname: " + hostname + "\n";

    return metaData;
}

/**
 * @brief Find ISO creation tool
 * @return path to ISO creation tool, or empty string if not found
 *
 * Find genisoimage, mkisofs, or xorriso on the system
 */
QString CloudInitIsoUtils::findISOCreationTool()
{
    QStringList tools;

    #ifdef Q_OS_WIN
    // On Windows, try to find mkisofs or genisoimage in common locations
    tools << "C:\\Program Files\\cdrtools\\mkisofs.exe"
          << "C:\\Program Files (x86)\\cdrtools\\mkisofs.exe"
          << "mkisofs.exe"
          << "genisoimage.exe";
    #else
    // On Unix-like systems, tools should be in PATH
    tools << "genisoimage" << "mkisofs" << "xorriso";
    #endif

    for (const QString &tool : tools) {
        QProcess process;
        process.start(tool, QStringList() << "--version");
        if (process.waitForFinished(3000) && process.exitCode() == 0) {
            qDebug() << "Found ISO creation tool:" << tool;
            return tool;
        }
    }

    qWarning() << "No ISO creation tool found (genisoimage, mkisofs, or xorriso)";
    return QString();
}

/**
 * @brief Create ISO file from directory
 * @param sourceDir, directory containing files to add to ISO
 * @param isoPath, path to the output ISO file
 * @return true if ISO was created successfully
 *
 * Create an ISO file using genisoimage, mkisofs, or xorriso
 */
bool CloudInitIsoUtils::createISO(const QString &sourceDir, const QString &isoPath)
{
    QString tool = findISOCreationTool();
    if (tool.isEmpty()) {
        return false;
    }

    QProcess process;
    QStringList args;

    if (tool.contains("xorriso")) {
        // xorriso has a different syntax
        args << "-as" << "mkisofs"
             << "-output" << QDir::toNativeSeparators(isoPath)
             << "-volid" << "cidata"
             << "-joliet"
             << "-rock"
             << QDir::toNativeSeparators(sourceDir);
    } else {
        // genisoimage and mkisofs use the same syntax
        args << "-output" << QDir::toNativeSeparators(isoPath)
             << "-volid" << "cidata"
             << "-joliet"
             << "-rock"
             << QDir::toNativeSeparators(sourceDir);
    }

    qDebug() << "Creating ISO with command:" << tool << args.join(" ");

    process.start(tool, args);
    if (!process.waitForFinished(30000)) {
        qWarning() << "ISO creation timed out";
        return false;
    }

    if (process.exitCode() != 0) {
        qWarning() << "ISO creation failed:" << process.readAllStandardError();
        return false;
    }

    return QFile::exists(isoPath);
}
