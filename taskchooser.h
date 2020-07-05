#ifndef TASKCHOOSER_H
#define TASKCHOOSER_H

#include "task.h"

#include <QDialog>
#include <vector>

namespace Ui {
class TaskChooser;
}

class TaskChooser : public QDialog
{
    Q_OBJECT

public:
    explicit TaskChooser(QWidget *parent = nullptr);
    ~TaskChooser();

private slots:
    void on_returnButton_clicked();

    void on_refreshButton_clicked();

    void on_OKbutton_clicked();

private:
    int loadTasks();

    void tasksOrCriticalAndReturn();

private:
    Ui::TaskChooser *ui;
    static constexpr const int s_OK = 0;
    static constexpr const int s_NO_TASKS = 1;

    std::vector<Task *> m_tasks;
};

#endif // TASKCHOOSER_H
