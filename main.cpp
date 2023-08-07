#include "mainwindow.h"
#include "chessboard.h"

#include <QApplication>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/chess.png"));

    MainWindow w;
    w.setWindowTitle(QSysInfo::prettyProductName() + " | e-Chessboard GUI");

    w.show();

    return a.exec();
}
