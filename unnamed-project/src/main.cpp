#include <QApplication>
#include <QCommandLineParser>

#include <memory>

#include "OpenGLWidget.h"
#include "PrimitiveGame.h"
#include "EditorWindow.h"


// http://www.ics.com/blog/qt-and-opengl-part-2-rendering-3d-model
// http://doc.qt.io/qt-5/qtopengl-hellogl2-example.html
// http://doc.qt.io/qt-5/cmake-manual.html

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("unnamed Project");
    QApplication::setApplicationVersion("build: " __DATE__ " " __TIME__);

    //setup command line argument parser
    QCommandLineParser parser;
    parser.setApplicationDescription("set description here"); //TODO: description
    parser.addVersionOption();
    QCommandLineOption editorOption("e", "use scene- and shader editor window");
    parser.addOption(editorOption);
    parser.addHelpOption();

    parser.process(app);

    bool useDeveloperView = parser.isSet(editorOption);

    if (!useDeveloperView)
    {

        OpenGLWidget widget(std::make_shared<PrimitiveGame>());
        widget.show();

        return app.exec();
    }
    //else:

    EditorWindow editorWindow(new OpenGLWidget(std::make_shared<PrimitiveGame>()));
    editorWindow.show();

    return app.exec();

    return 0;
}
