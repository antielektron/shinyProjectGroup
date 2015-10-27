#include <iostream>
#include <QApplication>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QWidget>

#include "myglwidget.h"
#include "mainwindow.h"

using namespace std;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();
}

