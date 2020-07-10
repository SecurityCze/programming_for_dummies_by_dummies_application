#include "ctaskstorage.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QObject>

#include "git2.h"

CTaskStorage::CTaskStorage()
{
    reloadTasks();
}

void CTaskStorage::reloadTasks() {
    git_libgit2_init();
    git_repository* repository = NULL;
    int gitReturnVal = git_repository_open(&repository, s_ROOT);

    if (gitReturnVal == GIT_ENOTFOUND) {
        qDebug() << "libgit2 error: "  << gitReturnVal << " - " << git_error_last()->message << " No such repository. Cloning repository to: " << s_ROOT;
        // TODO: CLONE

        return;
    } else if (gitReturnVal != GIT_OK) {
        qDebug() << "libgit2 error: "  << gitReturnVal << " - " << git_error_last()->message << " Fatal, on: " << s_ROOT;
        return;
    }

    git_remote* remote = NULL;
    gitReturnVal = git_remote_lookup(&remote, repository, s_GIT_REMOTE_NAME);
    if (gitReturnVal != GIT_OK) {
        qDebug() << "libgit2 error: " << gitReturnVal << " - " << git_error_last()->message << " Fatal, Git remote not found in: " << s_ROOT;
        return;
    }

    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    gitReturnVal = git_remote_connect(remote, GIT_DIRECTION_FETCH, &callbacks, nullptr, nullptr);
    if (gitReturnVal != GIT_OK) {
        qDebug() << "libgit2 error: " << gitReturnVal << " - " << git_error_last()->message << " Fatal, Git remote connect on: " << s_ROOT;
        return;
    }

    gitReturnVal = git_remote_download(remote, nullptr, nullptr);
    if (gitReturnVal != GIT_OK) {
        qDebug() << "libgit2 error: " << gitReturnVal << " - " << git_error_last()->message << " Fatal, Git download fail on: " << s_ROOT;
        return;
    }

    // TODO: finish pull

    qDebug() << "Finished git pull";
}

QString CTaskStorage::getTaskName(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "/" + s_TASK_NAME_FILENAME);
}

QString CTaskStorage::getTaskDescription(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "/" + s_TASK_DESCIPTION_FILENAME);
}

QString CTaskStorage::getTaskRuntimeExamples(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "/" + s_RUNTIME_EXAMPLES_FILENAME);
}

QString CTaskStorage::getTaskRecomendedDocumentation(const QString& taskID) const {
    return readFromUTF8File(s_ROOT + taskID + "/" + s_RECOMENDED_DOCUMENTATION_FILENAME);
}

std::list<SIDName> CTaskStorage::getTasks() const {
    QDir direcory(s_ROOT, "", QDir::Name, QDir::AllDirs | QDir::Readable | QDir::NoDotAndDotDot);
    QStringList taskIDs = direcory.entryList();

    std::list<SIDName> outTasks;
    for (const QString& taskID : taskIDs) {
        if (taskID.isEmpty() || taskID.at(0) == '.') // skips: folders without names / .folders
            continue;
        QString taskName = getTaskName(taskID);
        if (taskName.isEmpty()) // skips: tasks without taskname
            continue;

        outTasks.push_back(SIDName {taskID, taskName});
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
