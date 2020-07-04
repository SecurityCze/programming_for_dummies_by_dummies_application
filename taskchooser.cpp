#include "taskchooser.h"
#include "ui_taskchooser.h"

#include <QMessageBox>
#include <QDebug>

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
    delete ui;
}

void TaskChooser::on_returnButton_clicked()
{
    done(s_OK);
}

int TaskChooser::loadTasks()
{
    // TODO: load them from Github? probably cache them?
    ui->listOfTasks->clear();
    for (char i = '0'; i <= '9'; ++i) {
        ui->listOfTasks->addItem(QString(i));
    }
    ui->listOfTasks->update();

    tasksOrCriticalAndReturn();
    ui->listOfTasks->setCurrentRow(0);
    return ui->listOfTasks->count();
}

void TaskChooser::on_refreshButton_clicked()
{
    loadTasks();
}

void TaskChooser::on_OKbutton_clicked()
{
    auto item = ui->listOfTasks->currentItem();
    tasksOrCriticalAndReturn();
    qDebug() << item->text();
}

void TaskChooser::tasksOrCriticalAndReturn() {
    if (ui->listOfTasks->count() == 0) {
        QMessageBox::critical(this, "No tasks", "Sorry, no tasks are currently available.\nPlease come back later.");
        done(s_NO_TASKS);
    }
}