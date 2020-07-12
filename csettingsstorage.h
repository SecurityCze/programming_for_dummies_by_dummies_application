#ifndef CSETTINGSSTORAGE_H
#define CSETTINGSSTORAGE_H

#include <QString>

class CSettingsStorage
{
public:
    CSettingsStorage() = delete;

    static QString getTasksStoragePath();

    static void setTasksStoragePath(const QString &newPath);

    static void initStorage();

    static void resetToDefaults();

private:
    static void setDefaults(bool force = false);

    /**
     * @brief s_TASK_PATH_KEY key for loading task storage path
     */
    static constexpr const char *const s_TASK_PATH_KEY = "TASK_PATH";

    /**
     * @brief s_TASK_PATH_KEY
     */
    static constexpr const char *const s_TASK_PATH_DEFAULT = "../Programming_for_dummies_by_dummies_tasks";

    static constexpr const char *const s_ORGANIZATION_NAME = "S&K software";
    static constexpr const char *const s_APPLICATION_NAME = "Programming for dummies by dummies";
};

#endif // CSETTINGSSTORAGE_H
