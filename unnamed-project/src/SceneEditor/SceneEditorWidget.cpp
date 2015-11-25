#include "SceneEditor/SceneEditorWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/ObjectDetailsWidget.h"

#include "OpenGLWidget.h"

SceneEditorWidget::SceneEditorWidget(QWidget *parent) : QWidget(parent)
{
    m_game = std::make_shared<SceneEditorGame>();

    // QObjects free their children, when freed
    m_glWidget = new OpenGLWidget(m_game, this);
    m_glWidget->setMinimumSize(300, 300);

    m_objectDetails = new ObjectDetailsWidget(m_game, this);

    m_objectList = new QListView(this);

    // layout
    m_layout = new QHBoxLayout(this);

    m_layout->addWidget(m_glWidget, 1);
    m_layout->addWidget(m_objectDetails);
    m_layout->addWidget(m_objectList);

    this->setLayout(m_layout);
}

SceneEditorWidget::~SceneEditorWidget()
{
}
