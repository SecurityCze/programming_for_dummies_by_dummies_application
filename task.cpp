#include "task.h"
#include "ui_task.h"

#include <QFileDialog>
#include <QDebug>
#include <QTimeLine>
#include <QThread>

Task::Task(const QString& taskID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task),
    m_taskID(std::move(taskID))
{
    ui->setupUi(this);
    ui->groupboxMarks->hide();
    ui->progressBarSolution->hide();

    // TODO: decode taskID and load appropriate fields
    ui->taskName->setText(m_taskID);
    setWindowTitle(m_taskID);
}

Task::~Task()
{
    delete ui;
}

void Task::on_returnButton_clicked()
{
    done(s_OK);
}

void Task::on_fileButton_clicked()
{
    QString newFileName = QFileDialog::getOpenFileName(this, tr("Please chose file for marking"), tr(""), tr("C/C++ files(*.c, *.cpp);;All files(*)"));
    if (newFileName.isEmpty())
        return;
    m_fileName = newFileName;
    ui->fileName->setText(m_fileName);
    ui->markButton->setEnabled(true);
}

void Task::on_markButton_clicked()
{
    // TODO: marking process
    qDebug() << "Marking task: " << m_taskID << " from file: " << m_fileName;
    ui->progressBarSolution->show();

    for(int i = 0; i <= 100; ++i) {
        QThread::msleep(50);
        ui->progressBarSolution->setValue(i);
    }

    int mark = 110;
    showResult(mark, "Errors will be here:");
}

void Task::showResult(int mark, const QString& errors) {
    ui->groupboxMarks->show();
    ui->errors->setText(errors);
    ui->percentageMark->display(mark);
    if (mark <= 0)
        ui->percentageMark->setStyleSheet("QLCDNumber { color: red }");
    else if (mark < 100)
        ui->percentageMark->setStyleSheet("QLCDNumber { color: orange }");
    else
        ui->percentageMark->setStyleSheet("QLCDNumber { color: green }");
}
