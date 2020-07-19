#ifndef TASK_H
#define TASK_H

#include "ctaskstorage.h"
#include "csettingsstorage.h"
#include "cconstants.h"
#include "ccompiler.h"
#include "ctasktestprocesser.h"
#include "ctasknolimit.h"
#include "ctasktimelimited.h"

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include <QTimeLine>
#include <QThread>
#include <QList>

namespace Ui {
class Task;
}

class Task : public QDialog
{
    Q_OBJECT

public:
    explicit Task(const QString &taskID, const CTaskStorage& taskStorage, QWidget *parent = nullptr);
    ~Task();

private:
    enum COLOR_SHEET : int { BLACK = 0 , RED , GREEN , ORANGE };

private slots:
    void on_returnButton_clicked();

    void on_fileButton_clicked();

    void on_markButton_clicked();

    void showMark( int mark );

    void compilatorCheck( CCompiler::COMP_STATES compilatorState );

    void compilationCheck( CCompiler::COMPILATION compilationState );

    int  processTests( const QList< CTaskTestProcesser::CTaskSettings > & testList );

    QString makeColored( COLOR_SHEET color , const QString & line );

    void on_documentationBox_stateChanged(int arg1);

    void on_runtimeExamplesBox_stateChanged(int arg1);

    void on_taskCheckBox_stateChanged(int arg1);

    void on_markCheckBox_stateChanged(int arg1);

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
