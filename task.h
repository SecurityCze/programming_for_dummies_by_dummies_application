#ifndef TASK_H
#define TASK_H

#include <QDialog>

namespace Ui {
class Task;
}

class Task : public QDialog
{
    Q_OBJECT

public:
    explicit Task(const QString &taskID, QWidget *parent = nullptr);
    ~Task();

private slots:
    void on_returnButton_clicked();

    void on_fileButton_clicked();

    void on_markButton_clicked();

    void showResult(int mark, const QString& errors = "");

private:
    Ui::Task *ui;
    static constexpr const int s_OK = 0;
    QString m_taskID = "";
    QString m_fileName = "";
};

#endif // TASK_H
