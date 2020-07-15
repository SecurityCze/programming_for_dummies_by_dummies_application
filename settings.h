#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_taskStorageChooseDialogButton_clicked();

    void on_taskStorageApplyButton_clicked();

    void on_applyAllButton_clicked();

    void on_returnButton_clicked();

    void on_resetDefaultButton_clicked();

private:
    bool resetApplication();

    void applyStorageChange();

    Ui::Settings *ui;
};

#endif // SETTINGS_H
