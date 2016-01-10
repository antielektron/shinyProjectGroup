#include "SceneEditorWindow.h"

#include <QApplication>
#include <QAction>
#include <QFileDialog>

#include "SceneEditor/SceneEditorGame.h"
#include "SceneEditor/ObjectDetailsWidget.h"
#include "SceneEditor/ObjectListWidget.h"
#include "SceneEditor/ModelListWidget.h"
#include "SceneEditor/GlobalDetailsWidget.h"
#include "SceneEditor/AttributeWidget.h"
#include "SceneEditor/EventWidget.h"
#include "SceneEditor/AnimatorsWidget.h"

#include "Scene/Model.h"

#include "OpenGLWidget.h"

//DEBUG
#include <iostream>

//------------------------------------------------------------------------------
SceneEditorWindow::SceneEditorWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setMinimumSize(1024, 700);

    m_game = std::make_shared<SceneEditorGame>();

    // QObjects free their children, when freed
    m_glWidget = new OpenGLWidget(m_game, this);

    this->setCentralWidget(m_glWidget);

    m_objectDetails = new ObjectDetailsWidget(m_game, this);
    m_objectList = new ObjectListWidget(m_game, this);
    m_modelList = new ModelListWidget(m_game, this);
    m_globalDetails = new GlobalDetailsWidget(m_game, this);
    m_attributeWidget = new AttributeWidget(this);
    m_eventWidget = new EventWidget(this);
    m_animatorsWidget = new AnimatorsWidget(this);

    m_objectDetailsDock = new QDockWidget("Object Details", this);
    m_objectDetailsDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_objectDetailsDock->setWidget(m_objectDetails);

    m_objectListDock = new QDockWidget("Object List", this);
    m_objectListDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_objectListDock->setWidget(m_objectList);

    m_modelListDock = new QDockWidget("Model List", this);
    m_modelListDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_modelListDock->setWidget(m_modelList);

    m_globalDetailsDock = new QDockWidget("Global Details", this);
    m_globalDetailsDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_globalDetailsDock->setWidget(m_globalDetails);

    m_attributeWidgetDock = new QDockWidget("Attribute List", this);
    m_attributeWidgetDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_attributeWidgetDock->setWidget(m_attributeWidget);

    m_eventWidgetDock = new QDockWidget("Event List", this);
    m_eventWidgetDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_eventWidgetDock->setWidget(m_eventWidget);

    m_animatorsWidgetDock = new QDockWidget("Animators List", this);
    m_animatorsWidgetDock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    m_animatorsWidgetDock->setWidget(m_animatorsWidget);


    this->addDockWidget(Qt::LeftDockWidgetArea, m_objectDetailsDock);
    this->addDockWidget(Qt::RightDockWidgetArea, m_objectListDock);
    this->addDockWidget(Qt::RightDockWidgetArea, m_modelListDock);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_globalDetailsDock);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_attributeWidgetDock);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_eventWidgetDock);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_animatorsWidgetDock);

    this->tabifyDockWidget(m_globalDetailsDock, m_objectDetailsDock);
    this->tabifyDockWidget(m_attributeWidgetDock, m_eventWidgetDock);
    this->tabifyDockWidget(m_eventWidgetDock, m_animatorsWidgetDock);


    createActions();
    createToolbar();

    connectStuff();
}

//------------------------------------------------------------------------------
SceneEditorWindow::~SceneEditorWindow()
{
}

//------------------------------------------------------------------------------
void SceneEditorWindow::makeGlWidgetCurrent()
{
    m_glWidget->makeCurrent();
}

//------------------------------------------------------------------------------
void SceneEditorWindow::doneGlWidgetCurrent()
{
    m_glWidget->doneCurrent();
}

//------------------------------------------------------------------------------
Model *SceneEditorWindow::getCurrentModel()
{
    return m_currentModel;
}

//------------------------------------------------------------------------------
void SceneEditorWindow::createActions()
{
    QStyle *style = QApplication::style();

    m_loadScene = new QAction(style->standardIcon(QStyle::SP_DirOpenIcon),
                              "load Scene",
                              this);
    m_saveScene = new QAction(style->standardIcon(QStyle::SP_DialogSaveButton),
                              "save scene",
                              this);
    m_newScene = new QAction(style->standardIcon(QStyle::SP_FileIcon),
                             "new Scene",
                             this);

    m_loadScene->setShortcut(QKeySequence::Open);
    m_saveScene->setShortcut(QKeySequence::Save);
    m_newScene->setShortcut(QKeySequence::New);
}

//------------------------------------------------------------------------------
void SceneEditorWindow::createToolbar()
{
    m_toolbar = addToolBar("Toolbar");
    m_toolbar->setMovable(true);
    m_toolbar->addAction(m_newScene);
    m_toolbar->addAction(m_loadScene);
    m_toolbar->addAction(m_saveScene);
}

//------------------------------------------------------------------------------
void SceneEditorWindow::connectStuff()
{
    connect(m_game.get(), SIGNAL(modelsChanged()),
            m_objectDetails, SLOT(modelsChanged()));

    connect(m_game.get(), SIGNAL(currentObjectChanged()),
            m_objectDetails, SLOT(currentObjectChanged()));

    connect(m_game.get(), SIGNAL(objectsChanged()),
            m_objectList, SLOT(updateModelTree()));

    connect(m_modelList, SIGNAL(currentModelChanged(QString)),
            this, SLOT(onCurrentModelChanged(QString)));

    connect(m_objectList, SIGNAL(updateSceneObjectsRequest()),
            this, SLOT(onUpdateSceneObjectsRequest()));

    connect(m_attributeWidget, SIGNAL(attributeAdded(const QString &,
                                                     QVariant,
                                                     AttributeDatatype)),
            m_game.get(), SLOT(addAttribute(const QString &,
                                              QVariant,
                                              AttributeDatatype)));

    connect(m_animatorsWidget, SIGNAL(animatorAdded(std::unique_ptr<Animator> *)),
            m_game.get(), SLOT(addAnimator(std::unique_ptr<Animator> *)));

    connect(m_eventWidget, SIGNAL(eventAdded(const QString &,
                                             std::unique_ptr<PreconditionBase> *,
                                             std::unique_ptr<ActionBase> *)),
            m_game.get(), SLOT(addEvent(const QString &,
                                        std::unique_ptr<PreconditionBase> *,
                                        std::unique_ptr<ActionBase> *)));

    connect(m_game.get(), SIGNAL(singleAttributeAdded(GlobalState *, const QString &)),
            m_attributeWidget, SLOT(onSingleAttributeAdded(GlobalState *, const QString &)));

    connect(this, SIGNAL(globalStateModified(GlobalState *)),
            m_attributeWidget, SLOT(onAttributesChanged(GlobalState *)));

    connect(this, SIGNAL(globalStateModified(GlobalState *)),
            m_eventWidget, SLOT(onEventsChanged()));

    connect(this, SIGNAL(globalStateModified(GlobalState *)),
            m_animatorsWidget, SLOT(onAnimatorsChanged()));

    connect(m_game.get(), SIGNAL(sceneChanged()),
           this, SLOT(onSceneChanged()));

    connect (m_game.get(), SIGNAL(attributesChanged(GlobalState *)),
             m_attributeWidget, SLOT(onAttributesChanged(GlobalState *)));

    connect (m_game.get(), SIGNAL(eventsChanged(GlobalState *)),
             m_eventWidget, SLOT(onEventsChanged()));

    connect(m_game.get(), SIGNAL(animatorsChanged()),
            m_animatorsWidget, SLOT(onAnimatorsChanged()));

    connect(m_attributeWidget, SIGNAL(attributeDeleted(const QString &)),
            m_game.get(), SLOT(delAttribute(const QString &)));

    connect(m_eventWidget, SIGNAL(eventDeleted(const QString &)),
            m_game.get(), SLOT(delEvent(const QString &)));

    connect(m_animatorsWidget, SIGNAL(animatorDeleted(Animator *)),
            m_game.get(), SLOT(delAnimator(Animator *)));


    //connect Actions:
    connect(m_loadScene, SIGNAL(triggered()), this, SLOT(loadScene()));
    connect(m_saveScene, SIGNAL(triggered()), this, SLOT(saveScene()));
    connect(m_newScene, SIGNAL(triggered()), this, SLOT(newScene()));
}

//------------------------------------------------------------------------------
void SceneEditorWindow::loadScene()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Level"),
                                                    ".",
                                                    tr("Xml files (*.xml)"));
    if (filename.length() > 0)
    {
        makeGlWidgetCurrent();
        m_game->reset(std::unique_ptr<Scene>(new Scene(filename)));
        doneGlWidgetCurrent();

        // trigger resize event in order to update
        // the projection matrix
        m_game->resize(m_glWidget->geometry().width(),
                           m_glWidget->geometry().height());
    }

}

//------------------------------------------------------------------------------
void SceneEditorWindow::saveScene()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Level"),
                                                    ".",
                                                    tr("Xml files (*.xml)"));
    if (filename.length() > 0)
    {
        // TODO don't set dummy values!
        m_game->getScene()->setName("Shiny Scene");
        m_game->getScene()->setVersion("42");
        m_game->getScene()->setAuthor("Norby");
        m_game->getScene()->saveToFile(filename);
    }

}

//------------------------------------------------------------------------------
void SceneEditorWindow::newScene()
{
    makeGlWidgetCurrent();
    m_game->initialize();
    doneGlWidgetCurrent();
    // trigger resize event in order to update
    // the projection matrix
    m_game->resize(m_glWidget->geometry().width(),
                       m_glWidget->geometry().height());
}

//------------------------------------------------------------------------------
void SceneEditorWindow::onCurrentModelChanged(QString model)
{
    //TODO: maybe error handling?
    m_currentModel = m_game->getModelByName(model.toStdString());
}

//------------------------------------------------------------------------------
void SceneEditorWindow::onUpdateSceneObjectsRequest()
{
    m_game->getScene()->updateObjectList();
}

//------------------------------------------------------------------------------
void SceneEditorWindow::onSceneChanged()
{
    m_eventWidget->setGlobalState(m_game->getScene()->getGlobalState());
    m_animatorsWidget->setScene(m_game->getScene());
    emit globalStateModified(m_game.get()->getScene()->getGlobalState());
}

