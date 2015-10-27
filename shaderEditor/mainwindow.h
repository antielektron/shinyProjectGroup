#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

#include "myglwidget.h"
#include "codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QGroupBox *centralWidget;
    QGroupBox *codeEditorsBox;
    QGroupBox *statusBox;

    CodeEditor *fstextbox;
    CodeEditor *vstextbox;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;

    MyGLWidget *glwidget;

    QHBoxLayout *statusLayout;

    QLabel *statusLabel;

    QPushButton *acceptButton;

protected slots:
    void updateShader();

};

#endif // MAINWINDOW_H
