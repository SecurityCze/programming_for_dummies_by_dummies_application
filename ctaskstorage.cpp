#include "ctaskstorage.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>

CTaskStorage::CTaskStorage()
{
    reloadTasks();
}

void CTaskStorage::reloadTasks() {
    // TODO: load tasks from remote
}

QString CTaskStorage::getTaskName(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "\\" + s_TASK_NAME_FILENAME);
}

QString CTaskStorage::getTaskDescription(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "\\" + s_TASK_DESCIPTION_FILENAME);
}

QString CTaskStorage::getTaskRuntimeExamples(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "\\" + s_RUNTIME_EXAMPLES_FILENAME);
}

QString CTaskStorage::getTaskRecomendedDocumentation(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "\\" + s_RECOMENDED_DOCUMENTATION_FILENAME);
}

std::list<SIDName> CTaskStorage::getTasks() const {
    QDir direcory(s_ROOT, "", QDir::Name, QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList taskIDs = direcory.entryList();

    std::list<SIDName> outTasks;
    for (const QString& taskID : taskIDs) {
        outTasks.emplace_back(SIDName {.m_ID = taskID, .m_name = getTaskName(taskID)});
    }

    return outTasks;
}

QString CTaskStorage::readFromUTF8File(const QString &path) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open file: " << path;
        return "";
    }

    QTextStream in(&f);
    in.setCodec("UTF-8");
    QString result = in.readAll();
    f.close();
    return result;
}
