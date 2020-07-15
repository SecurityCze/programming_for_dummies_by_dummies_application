#include "csettingsstorage.h"

#include <QString>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>

QString CSettingsStorage::getTasksStoragePath()
{
    QSettings settings;
    return settings.value(s_TASK_PATH_KEY).toString();
}

void CSettingsStorage::setTasksStoragePath(const QString &newPath)
{
    QSettings settings;
    settings.setValue(s_TASK_PATH_KEY, newPath);
}

void CSettingsStorage::initStorage()
{
    QCoreApplication::setOrganizationName(s_ORGANIZATION_NAME);
    QCoreApplication::setApplicationName(s_APPLICATION_NAME);
    setDefaults(false);
}

void CSettingsStorage::resetToDefaults() {
    setDefaults(true);
}

void CSettingsStorage::setDefaults(bool force) {
    QSettings settings;
    if (force | !settings.contains(s_TASK_PATH_KEY)) {
        QDir dir(s_TASK_PATH_DEFAULT);
        dir.makeAbsolute();
        settings.setValue(s_TASK_PATH_KEY, dir.path());
    }
}