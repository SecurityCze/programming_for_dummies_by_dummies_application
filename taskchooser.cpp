#include "taskchooser.h"
#include "ui_taskchooser.h"
#include "task.h"

#include <QMessageBox>
#include <QDebug>
#include <QDir>

#include <list>

TaskChooser::TaskChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskChooser)
{
    ui->setupUi(this);
    if (loadTasks() == 0) // Close this window as soon as run loop is runnig
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}

TaskChooser::~TaskChooser()
{
    for (Task *currentTask : m_tasks) {
        delete currentTask;
    }

    delete ui;
}

void TaskChooser::on_returnButton_clicked()
{
    for (Task *currentTask: m_tasks) {
        currentTask->close();
        delete currentTask;
    }
    m_tasks.clear();
    done(s_OK);
}

int TaskChooser::loadTasks()
{
    ui->listOfTasks->clear();

    std::list<SIDName> tasks = m_taskStorage.getTasks();
    for (const SIDName &task : tasks) {
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setText(task.m_name);
        newItem->setData(Qt::UserRole, task.m_ID);
        ui->listOfTasks->addItem(newItem);
    }

    ui->listOfTasks->update();
    tasksOrCriticalAndReturn();
    ui->listOfTasks->setCurrentRow(0);
    return ui->listOfTasks->count();
}

void TaskChooser::on_refreshButton_clicked()
{
    m_taskStorage.reloadTasksFromRemote();
    loadTasks();
}

void TaskChooser::on_OKbutton_clicked()
{
    tasksOrCriticalAndReturn();
    auto item = ui->listOfTasks->currentItem();
    QString taskID = item->data(Qt::UserRole).toString();

    m_tasks.push_back(new Task(taskID, m_taskStorage, this));
    Task* selectedTask = m_tasks.back();
    selectedTask->show();
    selectedTask->exec();
}

void TaskChooser::tasksOrCriticalAndReturn() {
    if (ui->listOfTasks->count() == 0) {
        QMessageBox::critical(this, tr("No tasks"), tr("Sorry, no tasks are currently available.\nPlease come back later."));
        done(s_NO_TASKS);
    }
}
