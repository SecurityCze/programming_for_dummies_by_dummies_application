#include "task.h"
#include "ui_task.h"

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QTimeLine>
#include <QThread>
#include <QList>

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
    ui->taskText->setHtml(m_taskStorage.getTaskDescription(m_taskID));
    ui->runtimeExamplesEdit->setHtml(m_taskStorage.getTaskRuntimeExamples(m_taskID));
    ui->documentationEdit->setHtml(m_taskStorage.getTaskRecomendedDocumentation(m_taskID));
    ui->errors->setText("");
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
    QString newFileName = QFileDialog::getOpenFileName(this, tr("Please chose file for marking"), "", tr("C/C++ files (*.c *.cpp);;All files (*)"));
    if (newFileName.isEmpty())
        return;
    m_fileName = newFileName;
    ui->fileName->setText(m_fileName);
    ui->markButton->setEnabled(true);
}

void Task::on_markButton_clicked()
{
    // TODO: marking process -> move to another thread to not freze rest of application
    penalisation = false;
    qDebug() << "Marking task: " << m_taskID << " from file: " << m_fileName;
    ui->progressBarSolution->show();
    ui->groupboxMarks->show();
    ui->markCheckBox->setChecked(true);
    repaint();

    // check for compiler
    CCompiler::COMP_STATES compilatorState = CCompiler::IsAvailable();
    compilatorCheck( compilatorState );
    if( compilatorState != CCompiler::COMP_STATES::COMP_AVAILABLE ) return;

    uint32_t timeLimit = 0;

    // parse task list
    QList< CTaskTestProcesser::CTaskSettings > testList = CTaskTestProcesser::Parse( CSettingsStorage::getTasksStoragePath() + "/" + m_taskID , timeLimit );
    if( testList.size() == 0 )
    {
        ui->errors->append( CConstants::s_NO_TESTS_FOUND );
        return;
    }

    // compile and resolve
    CCompiler::COMPILATION compilationState = CCompiler::Compile(m_fileName, CSettingsStorage::getTasksStoragePath() + "/" + m_taskID ,  {CCompiler::COMP_PARAMS::PEDANTIC, CCompiler::COMP_PARAMS::WALL} , timeLimit );
    compilationCheck( compilationState );
    if( compilationState == CCompiler::COMPILATION::FAILED ) return;

    int passedTests = processTests( testList );

    int mark = ( static_cast< float >( passedTests ) / ( testList.size() * 2 ) ) * 100;
    if( penalisation ) mark *= 1.0f - ( static_cast< float >( s_COMPILATION_ERR_PENALTY_PERCENT ) / 100 );

    if( mark <= 0 )       ui->errors->append( makeColored( COLOR_SHEET::RED , "<b>Final score: " + QString::number( mark ) + "%</b>" ) );
    else if( mark < 100 ) ui->errors->append( makeColored( COLOR_SHEET::ORANGE , "<b>Final score: " + QString::number( mark ) + "%</b>" ) );
    else                  ui->errors->append( makeColored( COLOR_SHEET::GREEN , "<b>Final score: " + QString::number( mark ) + "%</b>" ) );
    showMark( mark );
}

void Task::showMark( int mark )
{
    ui->groupboxMarks->show();
    ui->percentageMark->display(mark);
    if (mark <= 0)
        ui->percentageMark->setStyleSheet("QLCDNumber { color: red }");
    else if (mark < 100)
        ui->percentageMark->setStyleSheet("QLCDNumber { color: orange }");
    else
        ui->percentageMark->setStyleSheet("QLCDNumber { color: green }");
}

void Task::compilatorCheck( CCompiler::COMP_STATES compilatorState )
{
    qDebug() << "Compiler present? (0-OK): " << compilatorState;
    if( compilatorState != CCompiler::COMP_STATES::COMP_AVAILABLE )
        ui->errors->setText( makeColored( COLOR_SHEET::RED , CConstants::s_ERROR_COMPILER_ERR ) );
}

void Task::compilationCheck( CCompiler::COMPILATION compilationState )
{
    qDebug() << "Compilation status: (20-OK, 21-Warnings, 22-FAIL)" << compilationState;
    if( compilationState == CCompiler::COMPILATION::FAILED )
    {
        ui->errors->setText( makeColored( COLOR_SHEET::RED , CConstants::s_ERROR_COMP_FAILED ) );
        showMark( 0 );
    }
    else if( compilationState == CCompiler::COMPILATION::WITH_WARNINGS )
    {
        ui->errors->setText( makeColored( COLOR_SHEET::ORANGE , QString( CConstants::s_ERROR_COMP_WARNS ) + QString::number( s_COMPILATION_ERR_PENALTY_PERCENT ) + QString( "%" ) ) );
        penalisation = true;
    }
    else if( compilationState == CCompiler::COMPILATION::SUCCESSFUL )
        ui->errors->setText( makeColored( COLOR_SHEET::GREEN , CConstants::s_COMPILATION_SUCCES ) );
    ui->errors->append( "" );
}

int Task::processTests( const QList< CTaskTestProcesser::CTaskSettings > & testList )
{
    int passed = 0, nthTest = 1;
    CTaskState taskState;
    CTaskAbstract * task = nullptr;

    for( const auto & test : testList )
    {
        ui->progressBarSolution->setValue( ( static_cast<float>(nthTest) / testList.size() ) * 100 );
        ui->errors->append("<b>Test #" + QString::number( nthTest ) + "</b>" );
        repaint();

        if( test.m_timeLimit )
        {
            qDebug() << "allocating task #" << QString::number( nthTest ) << " timeLimited";
            task = new CTaskTimeLimited( test.m_testName , CSettingsStorage::getTasksStoragePath() + "/" + m_taskID , test.m_timeLimit );
        }
        else
        {
            qDebug() << "allocating task #" << QString::number( nthTest ) << " notLimited";
            task = new CTaskNoLimit( test.m_testName , CSettingsStorage::getTasksStoragePath() + "/" + m_taskID );
        }
        if( test.m_memLimit ) task->SetMemLimit( test.m_memLimit );

        taskState = task->Process();

        if( taskState.GetTaskState() == CTaskState::TASK_STATE::FAILED )
        {
            ui->errors->append( makeColored( COLOR_SHEET::RED , "Test #" + QString::number( nthTest ) + " failed." ) );
            ui->errors->append( makeColored( COLOR_SHEET::RED , "Mark: 0%" ) );
            if( taskState.GetTaskMessage().size() )
            {
                ui->errors->append( makeColored( COLOR_SHEET::RED , "Error message: " ) );
                ui->errors->append( makeColored( COLOR_SHEET::RED , taskState.GetTaskMessage() ) );
            }
            ui->errors->append("");
        }
        else if( taskState.GetTaskState() == CTaskState::TASK_STATE::PART_FAIL )
        {
            ui->errors->append( makeColored( COLOR_SHEET::ORANGE , "Test #" + QString::number( nthTest ) + " passed with some errors." ) );
            ui->errors->append( makeColored( COLOR_SHEET::ORANGE ,  "Mark: 50%" ) );
            if( taskState.GetTaskMessage().size() )
            {
                ui->errors->append( makeColored( COLOR_SHEET::ORANGE , "Error message: " ) );
                ui->errors->append( makeColored( COLOR_SHEET::ORANGE , taskState.GetTaskMessage() ) );
            }
            ui->errors->append("");
            passed += 1;
        }
        else if( taskState.GetTaskState() == CTaskState::TASK_STATE::SUCCESSFUL )
        {
            ui->errors->append( makeColored( COLOR_SHEET::GREEN , "Test #" + QString::number( nthTest ) + " passed" ) );
            ui->errors->append( makeColored( COLOR_SHEET::GREEN , "Mark: 100%" ) );
            ui->errors->append("");
            passed += 2;
        }
        nthTest++;
        delete task;
    }
    return passed;
}

QString Task::makeColored( COLOR_SHEET color , const QString & line )
{
    QString span = "<span style=\"color: ";
    if( color == COLOR_SHEET::BLACK )       span += "black;\">";
    else if( color == COLOR_SHEET::RED  )   span += "red;\">";
    else if( color == COLOR_SHEET::GREEN )  span += "green;\">";
    else if( color == COLOR_SHEET::ORANGE ) span += "orange;\">";
    return ( span + line + "</span>" );
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

void Task::on_taskCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->taskText->show();
    else
        ui->taskText->hide();
}

void Task::on_markCheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->groupboxMarks->show();
    else
        ui->groupboxMarks->hide();
}
