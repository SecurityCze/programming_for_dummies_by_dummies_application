#ifndef MAINMENU_H
#define MAINMENU_H

#include "taskchooser.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenu; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    static constexpr const int RESTART_SIGNAL = -1;

public:
    MainMenu(QWidget *parent = nullptr);

    ~MainMenu();

private slots:
    void on_actionAbout_QY_triggered();

    void on_actionAbout_triggered();

    void on_tasksButton_clicked();

    void on_exitButton_clicked();

    void on_settingsButton_clicked();

    void on_aboutQtButton_clicked();

    void on_aboutButton_clicked();

private:
    Ui::MainMenu *ui;
    TaskChooser* m_taskChooser;
};
#endif // MAINMENU_H
