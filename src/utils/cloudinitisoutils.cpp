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
        // Hash the password using SHA-512 for security
        QString hashedPassword = hashPassword(machine->getCloudInitPassword());
        userData += "    passwd: " + hashedPassword + "\n";
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
 * @brief Hash password using SHA-512 crypt format
 * @param password, plain text password to hash
 * @return hashed password in crypt format
 *
 * Generate a SHA-512 hashed password compatible with Linux /etc/shadow format
 * This is more secure than storing plain text passwords
 *
 * Attempts to use system mkpasswd tool for proper crypt() hashing
 * Falls back to simplified SHA-512 if mkpasswd is not available
 */
QString CloudInitIsoUtils::hashPassword(const QString &password)
{
    // Try to use system mkpasswd for proper crypt() hashing
    QProcess mkpasswd;

    #ifdef Q_OS_UNIX
    // Try mkpasswd (available on Debian/Ubuntu)
    mkpasswd.start("mkpasswd", QStringList() << "--method=sha-512" << password);
    if (mkpasswd.waitForFinished(5000) && mkpasswd.exitCode() == 0) {
        QString hash = QString::fromUtf8(mkpasswd.readAllStandardOutput()).trimmed();
        if (!hash.isEmpty() && hash.startsWith("$6$")) {
            qDebug() << "Password hashed using mkpasswd";
            return hash;
        }
    }

    // Try python3 with passlib (fallback)
    QProcess python;
    python.start("python3", QStringList() << "-c"
                 << QString("from passlib.hash import sha512_crypt; print(sha512_crypt.hash('%1'))").arg(password));
    if (python.waitForFinished(5000) && python.exitCode() == 0) {
        QString hash = QString::fromUtf8(python.readAllStandardOutput()).trimmed();
        if (!hash.isEmpty() && hash.startsWith("$6$")) {
            qDebug() << "Password hashed using Python passlib";
            return hash;
        }
    }
    #endif

    #ifdef Q_OS_WIN
    // On Windows, try PowerShell with passlib if Python is installed
    QProcess python;
    python.start("python", QStringList() << "-c"
                 << QString("from passlib.hash import sha512_crypt; print(sha512_crypt.hash('%1'))").arg(password));
    if (python.waitForFinished(5000) && python.exitCode() == 0) {
        QString hash = QString::fromUtf8(python.readAllStandardOutput()).trimmed();
        if (!hash.isEmpty() && hash.startsWith("$6$")) {
            qDebug() << "Password hashed using Python passlib";
            return hash;
        }
    }
    #endif

    // Fallback: simplified SHA-512 implementation
    qWarning() << "mkpasswd/passlib not found, using simplified SHA-512 hash";
    qWarning() << "This may not be fully compatible with all systems";

    // Generate a random salt (16 characters, base64 charset)
    QString salt;
    const QString saltChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    for (int i = 0; i < 16; ++i) {
        salt += saltChars.at(QRandomGenerator::global()->bounded(saltChars.length()));
    }

    // Perform multiple rounds of hashing (simplified version of crypt)
    QByteArray data = (password + salt).toUtf8();
    QByteArray hash = data;

    // Do 5000 rounds of hashing (crypt typically uses 5000-999999 rounds)
    for (int i = 0; i < 5000; ++i) {
        hash = QCryptographicHash::hash(hash + data, QCryptographicHash::Sha512);
    }

    // Convert to base64 using the crypt alphabet
    QString hashB64 = hash.toBase64();

    // Format: $6$ indicates SHA-512, $rounds=5000$ indicates iterations, then salt$ and hash
    return QString("$6$rounds=5000$%1$%2").arg(salt, hashB64);
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
