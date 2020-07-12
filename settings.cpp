#include "settings.h"
#include "ui_settings.h"
#include "csettingsstorage.h"
#include "mainmenu.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDebug>


Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->taskStorageChosenLocation->setText(CSettingsStorage::getTasksStoragePath());
    ui->taskStorageApplyButton->setEnabled(false);

    ui->applyAllButton->setEnabled(false);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_taskStorageChooseDialogButton_clicked()
{
    const QString &currentStorageDirectory = ui->taskStorageChosenLocation->text();
    QString newStorageDirectory = QFileDialog::getExistingDirectory(this, tr("Choose directory for task storage"), currentStorageDirectory, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (newStorageDirectory.isEmpty() || QDir(newStorageDirectory) == QDir(currentStorageDirectory))
        return;

    ui->taskStorageChosenLocation->setText(newStorageDirectory);
    ui->taskStorageApplyButton->setEnabled(true);
    ui->applyAllButton->setEnabled(true);
}

void Settings::applyStorageChange()
{
    if (ui->taskStorageDeleteCurrentBox->isChecked()) {
        qDebug() << "Deleting: " << CSettingsStorage::getTasksStoragePath();
        QDir dir(CSettingsStorage::getTasksStoragePath());
        qDebug() << "Remove status (true succes): " << dir.removeRecursively();
    }

    CSettingsStorage::setTasksStoragePath(ui->taskStorageChosenLocation->text());
}

void Settings::on_taskStorageApplyButton_clicked()
{
    if (!resetApplication())
        return;
    applyStorageChange();
    QApplication::exit(MainMenu::RESTART_SIGNAL);
}

bool Settings::resetApplication() {
    auto reply = QMessageBox::question(this, tr("Application needs to restart"), tr("To apply changes application needs to be restarted."), QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
    return reply == QMessageBox::Ok;
}

void Settings::on_applyAllButton_clicked()
{
    if (!resetApplication())
        return;

    // Apply all settings that are enabled
    if (ui->taskStorageApplyButton->isEnabled())
        applyStorageChange();

    QApplication::exit(MainMenu::RESTART_SIGNAL);
}

void Settings::on_returnButton_clicked()
{
    done(0);
}

void Settings::on_resetDefaultButton_clicked()
{
    if (!resetApplication())
        return;

    CSettingsStorage::resetToDefaults();

    QApplication::exit(MainMenu::RESTART_SIGNAL);
}
