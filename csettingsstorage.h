#ifndef CSETTINGSSTORAGE_H
#define CSETTINGSSTORAGE_H

#include <QString>
#include <QFont>

class CSettingsStorage
{
public:
    CSettingsStorage() = delete;

    static QString getTasksStoragePath();

    static void setTasksStoragePath(const QString &newPath);

    static QFont getFont();

    static void setFont(const QFont &newFont);

    static void initStorage();

    static void resetToDefaults();

private:
    static void setDefaults(bool force = false);

    /**
     * @brief s_TASK_PATH_KEY key for loading task storage path
     */
    static constexpr const char *const s_TASK_PATH_KEY = "TASK_PATH";

    /**
     * @brief s_FONT_KEY key for loading font
     */
    static constexpr const char *const s_FONT_KEY = "FONT";

    /**
     * @brief s_TASK_PATH_DEFAULT default path for task storage
     */
    static constexpr const char *const s_TASK_PATH_DEFAULT = "../Programming_for_dummies_by_dummies_tasks";

    /**
     * @brief s_TASK_PATH_DEFAULT default path for task storage
     */
    static constexpr const char *const s_FONT_DEFAULT = "MS Shell Dlg 2,7.8,-1,5,50,0,0,0,0,0";

    /**
     * @brief s_ORGANIZATION_NAME name of organization
     */
    static constexpr const char *const s_ORGANIZATION_NAME = "S&K software";

    /**
     * @brief s_APPLICATION_NAME Name of application
     */
    static constexpr const char *const s_APPLICATION_NAME = "Programming for dummies by dummies";
};

#endif // CSETTINGSSTORAGE_H
