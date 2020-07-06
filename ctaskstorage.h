#ifndef CTASKSTORAGE_H
#define CTASKSTORAGE_H

#include <QString>
#include <list>

struct SIDName {
    QString m_ID;
    QString m_name;
};

class CTaskStorage
{
public:
    CTaskStorage();
    CTaskStorage(const CTaskStorage&) = delete;
    CTaskStorage operator=(const CTaskStorage&) = delete;

    void reloadTasks();

    QString getTaskName(const QString& taskID) const;

    QString getTaskDescription(const QString& taskID) const;

    std::list<SIDName> getTasks() const;

private:
    static QString readFromUTF8File(const QString &path);

    /**
     * @brief s_ROOT path to root storage of tasks
     */
    static constexpr const char *const s_ROOT = "..\\Programming_for_dummies_by_dummies_application\\PfDbD_tasks\\";
};

#endif // CTASKSTORAGE_H
