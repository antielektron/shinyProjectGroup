#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QOpenGLVertexArrayObject>

#include <QString>
#include <vector>
#include <tuple>

enum class OpenglErrorType {
    noError,
    vertexShaderError,
    fragmentShaderError,
    linkingError,
    undefinedError //when this happens, little cute kittens are dying...
};

class MyGLWidget : public QOpenGLWidget
{

    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /**
     * @brief setShader
     * @param vs vertex Shader
     * @param fs fragment shader
     * @return Error type
     */
    OpenglErrorType createShaderProgram(QString vs, QString fs);

    static const QString defaultVertexShader;

    static const QString defaultFragmentShader;

    static const std::vector<GLfloat> defaultQuad;

public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void deleteShaderProg();



    QOpenGLShaderProgram* shaderProg;


    QMatrix4x4 m_proj;
    QMatrix4x4 m_world;
    QMatrix4x4 m_camera;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

    int mvMatrixLoc;

signals:

public slots:
};

#endif // MYGLWIDGET_H
