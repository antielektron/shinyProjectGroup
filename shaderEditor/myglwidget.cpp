#include "myglwidget.h"
#include <iostream>

const QString MyGLWidget::defaultVertexShader =
        "#version 130\n"
        "//Vertex Shader:\n"
        "\n"
        "in vec3 position; \n"
        "\n"
        "uniform mat4 mvpMatrix;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = mvpMatrix * vec4(position, 1.);\n"
        "}\n";

const QString MyGLWidget::defaultFragmentShader =
        "#version 130\n"
        "//Fragment Shader:\n"
        "\n"
        "uniform vec4 color;\n"
        "\n"
        "out vec4 outputColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   outputColor = vec4(1.,1.,1.,1.); // color;\n"
        "}\n";

const std::vector<GLfloat> MyGLWidget::defaultQuad =
    {-0.3f, -0.3f, 0.0f,
     0.3f, -0.3f, 0.0f,
     0.3f, 0.3f, 0.0f,
     0.3f, 0.3f, 0.0f,
     -0.3f, 0.3f, 0.0f,
     -0.3f, -0.3f, 0.0f};


MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    //nothing to do here!
    this->shaderProg = nullptr;

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(0);
    this->setFormat(format);
}

//=============================================================================

MyGLWidget::~MyGLWidget()
{
    cleanup();
}

//=============================================================================

void MyGLWidget::cleanup()
{
    makeCurrent();
    deleteShaderProg();
    std::cout << "cleaning" << std::endl;
    doneCurrent();
}

//=============================================================================

void MyGLWidget::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &MyGLWidget::cleanup);

    //glClearColor(0,0,0,1);

    createShaderProgram(MyGLWidget::defaultVertexShader, MyGLWidget::defaultFragmentShader);

    mvMatrixLoc = shaderProg->uniformLocation("mvpMatrix");

    //setup vao:
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    //setup vbo
    m_vbo.create();
    m_vbo.bind();
    //allocate 6 vertices for a quad:
    m_vbo.allocate(defaultQuad.data(), 6 * 3 * sizeof(GLfloat));

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    //fill with data:
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    m_vbo.release();

    m_camera.setToIdentity();
    m_camera.translate(0,0,-1);

    //debug info:
    auto version = context()->format().version();
    std::cout << "Using OpenGL Version " << version.first << "." << version.second << std::endl;

}

//=============================================================================

void MyGLWidget::deleteShaderProg()
{
    if (shaderProg)
    {

        delete shaderProg;
    }
    shaderProg = nullptr;
}

//=============================================================================

OpenglErrorType MyGLWidget::createShaderProgram(QString vs, QString fs)
{
    QOpenGLShaderProgram *newShaderProg = new QOpenGLShaderProgram();

    if (!newShaderProg->addShaderFromSourceCode(QOpenGLShader::Vertex, vs))
    {
        std::cerr << "Error while loading Vertex Shader" << std::endl;
        delete newShaderProg;
        return OpenglErrorType::vertexShaderError;
    }

    if (!newShaderProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fs))
    {
        std::cerr << "Error while loading Fragment shader" << std::endl;
        delete newShaderProg;
        return OpenglErrorType::fragmentShaderError;
    }


    newShaderProg->bindAttributeLocation("position",0);
    if (!newShaderProg->link())
    {
        std::cerr << "Error while linking shader Program!" << std::endl;
        delete newShaderProg;
        return OpenglErrorType::linkingError;
    }

    newShaderProg->bind();

    mvMatrixLoc = newShaderProg->uniformLocation("mvpMatrix");

    newShaderProg->release();
    shaderProg = newShaderProg;
    return OpenglErrorType::noError;



}

//=============================================================================

void MyGLWidget::resizeGL(int width, int height)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(width) / height, 0.01f, 100.0f);
}

//=============================================================================

void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shaderProg->bind();
    shaderProg->setUniformValue(mvMatrixLoc, m_proj * m_camera);

    m_vbo.bind();

    f->glDrawArrays(GL_TRIANGLES,0,6);
}

//=============================================================================

QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50,50);
}

//=============================================================================

QSize MyGLWidget::sizeHint() const
{
    return QSize(400,400);
}

