#include "task.h"
#include "ui_task.h"
#include "ccompiler.h"

#include <QFileDialog>
#include <QDebug>
#include <QTimeLine>
#include <QThread>

Task::Task(const QString& taskID, const CTaskStorage& taskStorage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Task),
    m_taskID(std::move(taskID)),
    m_taskStorage(taskStorage)
{
    ui->setupUi(this);
    ui->groupboxMarks->hide();
    ui->progressBarSolution->hide();
    ui->documentationEdit->hide();
    ui->runtimeExamplesEdit->hide();

    // TODO: decode taskID and load appropriate fields
    QString taskTextName = m_taskStorage.getTaskName(m_taskID);
    ui->taskName->setText(taskTextName);
    setWindowTitle(tr("PfDbD") + " - " + taskTextName);
    ui->taskText->setMarkdown(m_taskStorage.getTaskDescription(m_taskID));
    ui->runtimeExamplesEdit->setMarkdown(m_taskStorage.getTaskRuntimeExamples(m_taskID));
    ui->documentationEdit->setMarkdown(m_taskStorage.getTaskRecomendedDocumentation(m_taskID));
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
    QString newFileName = QFileDialog::getOpenFileName(this, tr("Please chose file for marking"), "", tr("C/C++ files (*.c, *.cpp);;All files (*)"));
    if (newFileName.isEmpty())
        return;
    m_fileName = newFileName;
    ui->fileName->setText(m_fileName);
    ui->markButton->setEnabled(true);
}

void Task::on_markButton_clicked()
{
    // TMP - compiler develop - START
    CCompiler comp;
    qDebug() << "HEHEHEHEHHEHEHEHEHHE " << comp.IsAvailable();
    qDebug() << " HEHEHEH END";
    // TMP - compiler develop - END

    // TODO: marking process -> move to another thread to not freze rest of application
    qDebug() << "Marking task: " << m_taskID << " from file: " << m_fileName;
    ui->progressBarSolution->show();

    // Just a DUMMY -> to show how it could look when marking
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

void Task::on_documentationBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->documentationEdit->show();
    else
        ui->documentationEdit->hide();
}

void Task::on_runtimeExamplesBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->runtimeExamplesEdit->show();
    else
        ui->runtimeExamplesEdit->hide();
}
