#ifndef TASK_H
#define TASK_H

#include "ctaskstorage.h"
#include "csettingsstorage.h"
#include "cconstants.h"

#include <QDialog>

namespace Ui {
class Task;
}

class Task : public QDialog
{
    Q_OBJECT

public:
    explicit Task(const QString &taskID, const CTaskStorage& taskStorage, QWidget *parent = nullptr);
    ~Task();

private slots:
    void on_returnButton_clicked();

    void on_fileButton_clicked();

    void on_markButton_clicked();

    void showResult(int mark, const QString& errors = "");

    void showMark( int mark );

    void on_documentationBox_stateChanged(int arg1);

    void on_runtimeExamplesBox_stateChanged(int arg1);

private:
    Ui::Task *ui;
    static constexpr const int s_OK = 0;
    static constexpr const int s_COMPILATION_ERR_PENALTY_PERCENT = 15;
    QString m_taskID;
    QString m_fileName = "";
    const CTaskStorage& m_taskStorage;
    bool penalisation = false;
};

#endif // TASK_H
