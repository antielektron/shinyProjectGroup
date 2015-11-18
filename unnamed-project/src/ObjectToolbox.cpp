#include "ObjectToolbox.h"
#include <iostream>
#include <QStyle>
#include <QApplication>
//=============================================================================

ObjectToolbox::ObjectToolbox(QWidget *parent):QDockWidget(parent)
{
    createActions();
    createToolbar();
    createColumnView();
    bakeUI();
}

//=============================================================================

ObjectToolbox::~ObjectToolbox()
{
    //nothing to do here...
}

//=============================================================================

void ObjectToolbox::createActions()
{

    //create Actions:
    QStyle* style = QApplication::style();

    m_addObjectAction = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_FileDialogNewFolder),"add Object", this));
    m_delObjectAction = std::unique_ptr<QAction>(new QAction(style->standardIcon(QStyle::SP_TrashIcon),"delete Object", this));

    m_addObjectAction->setShortcut(QKeySequence::New);
    m_delObjectAction->setShortcut(QKeySequence::Delete);

    connect(m_addObjectAction.get(), SIGNAL(triggered()), this, SLOT(addObject()));
    connect(m_delObjectAction.get(), SIGNAL(triggered()), this, SLOT(delObject()));

}

//=============================================================================

void ObjectToolbox::createToolbar()
{
    m_toolbar = std::unique_ptr<QToolBar>(new QToolBar("Object Tools",this));
    m_toolbar->setMovable(false);
    m_toolbar->addAction(m_addObjectAction.get());
    m_toolbar->addAction(m_delObjectAction.get());
}

//=============================================================================

void ObjectToolbox::createColumnView()
{
    //TODO
}

//=============================================================================

void ObjectToolbox::bakeUI()
{
    m_layout = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(this));
    m_multiWidget = std::unique_ptr<QWidget>(new QWidget(this));
    m_multiWidget->setLayout(m_layout.get());

    m_layout->addWidget(m_toolbar.get());
    //TODO: add column view

    this->setWidget(m_multiWidget.get());

}

//=============================================================================

void ObjectToolbox::addObject()
{
    std::cout << "TODO: add object" << std::endl;
}

//=============================================================================

void ObjectToolbox::delObject()
{
    std::cout << "TODO: delete Object" << std::endl;
}
