#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "taskchooser.h"

#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::on_actionAbout_QY_triggered()
{
    QMessageBox::aboutQt(this, tr("About Qt framework"));
}

void MainMenu::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About this application"), tr("This application was created as a tool to help our friends learn programming..."));
}

void MainMenu::on_tasksButton_clicked()
{
    TaskChooser tasks(nullptr);
    hide();
    tasks.exec();
    show();
}

void MainMenu::on_exitButton_clicked()
{
    auto reply = QMessageBox::question(this, tr("Exit?"), tr("Do you wish to exit the application?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}
