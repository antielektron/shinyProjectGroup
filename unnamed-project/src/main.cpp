#include <QApplication>
#include <QCommandLineParser>

#include <memory>

#include "OpenGLWidget.h"
#include "PrimitiveGame.h"
#include "EditorWindow.h"
#include "SceneEditor/SceneEditorWidget.h"


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

    QCommandLineOption shaderEditorOption("e", "Start the shader editor");
    parser.addOption(shaderEditorOption);

    QCommandLineOption sceneEditorOption("sceneeditor", "Start the scene editor");
    parser.addOption(sceneEditorOption);

    parser.addHelpOption();
    parser.process(app);

    if (parser.isSet(shaderEditorOption))
    {
        EditorWindow editorWindow(new OpenGLWidget(std::make_shared<PrimitiveGame>()));
        editorWindow.show();

        return app.exec();
    }
    else if (parser.isSet(sceneEditorOption))
    {
        SceneEditorWidget sceneEditor;
        sceneEditor.show();

        return app.exec();
    }
    else
    {
        // Start the regular primitive game ..
        OpenGLWidget widget(std::make_shared<PrimitiveGame>());
        widget.show();

        return app.exec();
    }
}
