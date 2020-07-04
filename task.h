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
    explicit Task(QWidget *parent = nullptr);
    ~Task();

    void setTask(const QString& taskId);

private slots:
    void on_returnButton_clicked();

private:
    Ui::Task *ui;
    static constexpr const int s_OK = 0;
};

#endif // TASK_H
