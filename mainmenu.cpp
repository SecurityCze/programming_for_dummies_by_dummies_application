#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "taskchooser.h"
#include "csettingsstorage.h"
#include "settings.h"

#include <QMessageBox>
#include <QSettings>
#include <QDebug>

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    CSettingsStorage::initStorage();
    qDebug() << "Setting font: " << CSettingsStorage::getFont();
    QApplication::setFont(CSettingsStorage::getFont());
    m_taskChooser = new TaskChooser;
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
    hide();
    m_taskChooser->loadTasks();
    m_taskChooser->move(pos());
    m_taskChooser->exec();
    show();
}

void MainMenu::on_exitButton_clicked()
{
    auto reply = QMessageBox::question(this, tr("Exit?"), tr("Do you wish to exit the application?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}

void MainMenu::on_settingsButton_clicked()
{
    Settings settingsWindow(this);

    settingsWindow.exec();
}

void MainMenu::on_aboutQtButton_clicked()
{
    QMessageBox::aboutQt(this, tr("About Qt framework."));
}

void MainMenu::on_aboutButton_clicked()
{
    QMessageBox::about(this, tr("About creators."), tr("Who are we? We are two students from Czech republic who were asked to help their friends with programming. So here we are. We have created this \"masterpiece\" to prepare them for tough life with beatiful C and C++.\n\nContact:\nEmail: sandksoftware@seznam.cz"));
}
