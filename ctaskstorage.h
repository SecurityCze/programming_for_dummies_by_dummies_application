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

    QString getTaskRuntimeExamples(const QString& taskID) const;

    QString getTaskRecomendedDocumentation(const QString& taskID) const;

    std::list<SIDName> getTasks() const;

private:
    static QString readFromUTF8File(const QString &path);

    /**
     * @brief s_ROOT path to root storage of tasks
     */
    static constexpr const char *const s_ROOT = "..\\Programming_for_dummies_by_dummies_application\\PfDbD_tasks\\";

    static constexpr const char *const s_TASK_NAME_FILENAME = "name.txt";
    static constexpr const char *const s_TASK_DESCIPTION_FILENAME = "task.md";
    static constexpr const char *const s_RUNTIME_EXAMPLES_FILENAME = "runtime.md";
    static constexpr const char *const s_RECOMENDED_DOCUMENTATION_FILENAME = "documentation.md";
};

#endif // CTASKSTORAGE_H
