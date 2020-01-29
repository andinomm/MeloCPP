#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QSplashScreen>
#include <QVBoxLayout>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixSplash(":/basic/splash.png");
    QSplashScreen splash;
    splash.setPixmap(pixSplash);





    splash.show();



    MainWindow w;

    QTimer::singleShot(2500,&splash,SLOT(close()));
    QTimer::singleShot(2500,&w,SLOT(show()));



   // splash.finish(&w);


    return a.exec();
}
