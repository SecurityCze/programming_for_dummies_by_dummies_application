#include "ctaskstorage.h"
#include "csettingsstorage.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QObject>
#include <QSettings>
#include <QCoreApplication>

#include "git2.h"

CTaskStorage::CTaskStorage(): m_taskPath(CSettingsStorage::getTasksStoragePath())
{
    reloadTasksFromRemote();
}

bool CTaskStorage::reloadTasksFromRemote() {
    git_libgit2_init();
    git_repository* repository = nullptr;
    git_remote* remote = nullptr;
    git_object *headObject = nullptr;
    git_reference *remoteHEADReference = nullptr;
    git_reference *targetReference = nullptr;
    git_reference *newTargetReference = nullptr;

    int gitReturnVal = git_repository_open(&repository, m_taskPath.toStdString().c_str());

    if (gitReturnVal == GIT_ENOTFOUND) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "No such repository, attempting to clone");

        gitReturnVal = git_clone(&repository, s_REMOTE_URL, m_taskPath.toStdString().c_str(), nullptr);
    }

    if (gitReturnVal != GIT_OK) {   // Clone fail or other error while opening
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, no respositry, unable to create new one");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    gitReturnVal = git_remote_lookup(&remote, repository, s_GIT_REMOTE_NAME);
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, git remote not found");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    gitReturnVal = git_remote_fetch(remote, nullptr, nullptr, "pull");
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, git fatch fail");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    gitReturnVal = git_reference_lookup(&remoteHEADReference, repository, s_GIT_REMOTE_REFERENCE);
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, git reset --hard fail");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    /* HEAD exists, just lookup and resolve */
    gitReturnVal = git_repository_head(&targetReference, repository);
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, failed to get HEAD reference");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    /* Move the target reference to the target OID */
    gitReturnVal = git_reference_set_target(&newTargetReference, targetReference, git_reference_target(remoteHEADReference), NULL);
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, failed to move HEAD reference");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    gitReturnVal = git_revparse_single(&headObject, repository, "HEAD");
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, unable to get head object");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    gitReturnVal = git_reset(repository, headObject, GIT_RESET_HARD, nullptr);
    if (gitReturnVal != GIT_OK) {
        libgit2ErrorDebug(gitReturnVal, git_error_last()->message, "Fatal, git reset --hard fail");
        libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
        return false;
    }

    libgit2cleanUp(repository, remote, headObject, remoteHEADReference, targetReference, newTargetReference);
    qDebug() << "Finished git pull";
    return true;
}

void CTaskStorage::libgit2cleanUp(git_repository *repository, git_remote *remote, git_object *object, git_reference *remoteHEAD, git_reference *target, git_reference *newTarger) {
    git_repository_free(repository);
    git_remote_free(remote);
    git_object_free(object);
    git_reference_free(remoteHEAD);
    git_reference_free(target);
    git_reference_free(newTarger);
    git_libgit2_shutdown();
}

QString CTaskStorage::getTaskName(const QString& taskID) const {
    return readFromUTF8File(m_taskPath + "/" + taskID + "/" + s_TASK_NAME_FILENAME);
}

QString CTaskStorage::getTaskDescription(const QString& taskID) const {
    return readFromUTF8File(m_taskPath + "/" + taskID + "/" + s_TASK_DESCIPTION_FILENAME);
}

QString CTaskStorage::getTaskRuntimeExamples(const QString& taskID) const {
    return readFromUTF8File(m_taskPath + "/" + taskID + "/" + s_RUNTIME_EXAMPLES_FILENAME);
}

QString CTaskStorage::getTaskRecomendedDocumentation(const QString& taskID) const {
    return readFromUTF8File(m_taskPath + "/" + taskID + "/" + s_RECOMENDED_DOCUMENTATION_FILENAME);
}

std::list<SIDName> CTaskStorage::getTasks() const {
    QDir direcory(m_taskPath, "", QDir::Name, QDir::AllDirs | QDir::Readable | QDir::NoDotAndDotDot);
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

void CTaskStorage::libgit2ErrorDebug(int errorCode, const char* const message, const char* const moreInfo) {
    qDebug() << "libgit2 error: " << errorCode << " <" << message << "> on repository <" << m_taskPath << ">\t" << moreInfo;
}
