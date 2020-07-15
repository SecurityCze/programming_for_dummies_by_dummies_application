#include "mainmenu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    int returnCode = 0;
    do {
        QApplication a(argc, argv);
        MainMenu mainWindow;
        mainWindow.show();
        returnCode = a.exec();
    } while (returnCode == MainMenu::RESTART_SIGNAL);

    return returnCode;
}
