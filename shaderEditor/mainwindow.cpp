#include "mainwindow.h"
#include "glslhighlighter.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{


    centralWidget = new QGroupBox();
    codeEditorsBox = new QGroupBox(centralWidget);
    statusBox = new QGroupBox(codeEditorsBox);

    hlayout = new QHBoxLayout(centralWidget);
    vlayout = new QVBoxLayout(codeEditorsBox);

    statusLayout = new QHBoxLayout(statusBox);

    glwidget = new MyGLWidget(centralWidget);
    fstextbox = new CodeEditor(codeEditorsBox);
    vstextbox = new CodeEditor(codeEditorsBox);

    acceptButton = new QPushButton(statusBox);

    statusLabel = new QLabel(statusBox);

    setCentralWidget(centralWidget);
    centralWidget->setLayout(hlayout);
    codeEditorsBox->setLayout(vlayout);
    statusBox->setLayout(statusLayout);

    hlayout->addWidget(glwidget,1);

    hlayout->addWidget(codeEditorsBox,1);

    vlayout->addWidget(vstextbox,1);

    vlayout->addWidget(fstextbox,1);
    vlayout->addWidget(statusBox);

    statusLayout->addWidget(acceptButton);
    statusLayout->addWidget(statusLabel);

    //set Labels etc:
    acceptButton->setText("update shader");

    statusLabel->setText("status:");

    codeEditorsBox->setTitle("shader editor:");

    //set default shader code:
    vstextbox->setPlainText(MyGLWidget::defaultVertexShader);
    fstextbox->setPlainText(MyGLWidget::defaultFragmentShader);

    //set fancy highlighter:
    highlighterVS = new GLSLHighlighter(vstextbox->document());
    highlighterFS = new GLSLHighlighter(fstextbox->document());

    //connect signals:
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(updateShader()));

    //set Color values:
    allFineColor = QColor::fromRgb(210,255,210);
    errorColor = QColor::fromRgb(255,210,210);

}

//=============================================================================

MainWindow::~MainWindow()
{

    delete centralWidget;
    //delete hlayout;
    //delete glwidget;
    //delete textbox;
}

//=============================================================================

void MainWindow::updateShader()
{

    OpenglErrorType retVal;
    retVal = glwidget->createShaderProgram(vstextbox->toPlainText(), fstextbox->toPlainText());
    switch (retVal)
    {
    case OpenglErrorType::noError:
    {
        statusLabel->setText("all good");
        QPalette p = vstextbox->palette();
        p.setColor(QPalette::Base, allFineColor);
        vstextbox->setPalette(p);
        fstextbox->setPalette(p);
        break;
    }
    case OpenglErrorType::vertexShaderError:
    {
        statusLabel->setText("vertex Shader failed");
        QPalette p = vstextbox->palette();
        p.setColor(QPalette::Base, errorColor);
        vstextbox->setPalette(p);
        break;
    }
    case OpenglErrorType::fragmentShaderError:
    {
        statusLabel->setText("fragment Shader failed");
        QPalette p = fstextbox->palette();
        p.setColor(QPalette::Base, errorColor);
        fstextbox->setPalette(p);
        break;
    }
    case OpenglErrorType::linkingError:
    {
        statusLabel->setText("linking failed");
        break;
    }
    default:
    {
        statusLabel->setText("well, you're ******");
        break;
    }
    }

    glwidget->update();


}

