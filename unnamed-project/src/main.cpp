#include <QApplication>
#include <QCommandLineParser>

#include <memory>
#include <QFileDialog>

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
    parser.addOption(shaderEditorOption);

    QCommandLineOption sceneEditorOption("scene", "Start the scene editor");
    parser.addOption(sceneEditorOption);

    QCommandLineOption fullscreenOption("fullscreen", "start game in fullscreen");
    parser.addOption(fullscreenOption);

#ifdef HAVE_BULLET
    QCommandLineOption bulletGameOption("bullet", "Start the game!");
    parser.addOption(bulletGameOption);
#endif // HAVE_BULLET

    QCommandLineOption levelFileOption("level", "Specify level file to use", "file", "level/sphere_animation2.xml");
    parser.addOption(levelFileOption);

    parser.addHelpOption();
    parser.process(app);

    QString levelFile;

    if (parser.isSet(levelFileOption))
    {
        levelFile = parser.value(levelFileOption);
    }
    else
    {
        levelFile = QFileDialog::getOpenFileName(nullptr,
                                                 "Load Level",
                                                 "./level/",
                                                 "xml files (*.xml)");
    }

    if (parser.isSet(sceneEditorOption))
    {
        SceneEditorWindow sceneEditor(levelFile);
        sceneEditor.show();

        return app.exec();
    }
#ifdef HAVE_BULLET
    else if (parser.isSet(bulletGameOption))
    {
        if (parser.isSet(shaderEditorOption))
        {
            EditorWindow editorWindow(
                        new OpenGLWidget(std::make_shared<BulletGame>(levelFile)));
            editorWindow.show();

            return app.exec();
        }
        else
        {
            OpenGLWidget widget(std::make_shared<BulletGame>(levelFile));
            if (parser.isSet(fullscreenOption))
            {
                widget.showFullScreen();
            }

            else{
                widget.show();
            }
            return app.exec();
        }
    }
#endif // HAVE_BULLET
    else
    {
        if (parser.isSet(shaderEditorOption))
        {
            EditorWindow editorWindow(
                        new OpenGLWidget(std::make_shared<PrimitiveGame>()));
            editorWindow.show();

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
}
