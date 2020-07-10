#ifndef CTASKSTORAGE_H
#define CTASKSTORAGE_H

#include <QString>
#include <QObject>

#include <list>

struct SIDName {
    QString m_ID;
    QString m_name;
};

class CTaskStorage : public QObject {

    Q_OBJECT

public:
    CTaskStorage();
    CTaskStorage(const CTaskStorage&) = delete;
    CTaskStorage operator=(const CTaskStorage&) = delete;

    virtual ~CTaskStorage() = default;

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
    static constexpr const char *const s_ROOT = "../Programming_for_dummies_by_dummies_tasks/";
    //static constexpr const char *const s_ROOT = "C:/Users/Matej/Documents/00_CVUT/PS2/";
    static constexpr const char *const s_GIT_REMOTE_NAME = "origin";

    static constexpr const char *const s_TASK_NAME_FILENAME = "name.txt";
    static constexpr const char *const s_TASK_DESCIPTION_FILENAME = "task.md";
    static constexpr const char *const s_RUNTIME_EXAMPLES_FILENAME = "runtime.md";
    static constexpr const char *const s_RECOMENDED_DOCUMENTATION_FILENAME = "documentation.md";
};

#endif // CTASKSTORAGE_H
