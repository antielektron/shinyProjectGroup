#include <QApplication>
#include <QCommandLineParser>

#include <memory>

#include "OpenGLWidget.h"
#include "PrimitiveGame.h"
#include "BulletGame.h"
#include "ShaderEditor/EditorWindow.h"
#include "SceneEditor/SceneEditorWindow.h"


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

    QCommandLineOption shaderEditorOption("shader", "Start the shader editor");
    QCommandLineOption shaderEditorOption2("e", "Start the shader editor");
    parser.addOption(shaderEditorOption);
    parser.addOption(shaderEditorOption2);

    QCommandLineOption sceneEditorOption("scene", "Start the scene editor");
    parser.addOption(sceneEditorOption);

#ifdef HAVE_BULLET
    QCommandLineOption bulletGameOption("bullet", "Start the game!");
    parser.addOption(bulletGameOption);
#endif // HAVE_BULLET

    parser.addHelpOption();
    parser.process(app);

    if (parser.isSet(shaderEditorOption) || parser.isSet(shaderEditorOption2))
    {
        EditorWindow editorWindow(new OpenGLWidget(std::make_shared<PrimitiveGame>()));
        editorWindow.show();

        return app.exec();
    }
    else if (parser.isSet(sceneEditorOption))
    {
        SceneEditorWindow sceneEditor;
        sceneEditor.show();

        return app.exec();
    }
#ifdef HAVE_BULLET
    else if (parser.isSet(bulletGameOption))
    {
        // TODO have window (with debug ui)
        // TODO add command line argument for level file to use!
        OpenGLWidget widget(std::make_shared<BulletGame>("level/test.xml"));
        widget.show();

        return app.exec();
    }
#endif // HAVE_BULLET
    else
    {
        // Start the regular primitive game ..
        OpenGLWidget widget(std::make_shared<PrimitiveGame>());
        widget.show();

        return app.exec();
    }
}
