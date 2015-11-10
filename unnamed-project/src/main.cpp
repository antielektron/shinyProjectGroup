#include <QApplication>

#include <memory>

#include "OpenGLWidget.h"
#include "PrimitiveGame.h"


// http://www.ics.com/blog/qt-and-opengl-part-2-rendering-3d-model
// http://doc.qt.io/qt-5/qtopengl-hellogl2-example.html
// http://doc.qt.io/qt-5/cmake-manual.html

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    OpenGLWidget widget(std::make_shared<PrimitiveGame>());
    widget.show();

    return app.exec();
}
