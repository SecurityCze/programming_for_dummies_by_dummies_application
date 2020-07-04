#include "task.h"
#include "ui_task.h"

Task::Task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task)
{
    ui->setupUi(this);
}

Task::~Task()
{
    delete ui;
}


void Task::setTask(const QString& taskId) {
    // TODO: read task and set boxes
    ui->taskName->setText(taskId);
}

void Task::on_returnButton_clicked()
{
    done(s_OK);
}
