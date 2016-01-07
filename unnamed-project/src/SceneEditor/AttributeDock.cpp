#include "AttributeDock.h"

AttributeDock::AttributeDock(QWidget *parent) :
    QWidget(parent)
{
    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void AttributeDock::generateWidgets()
{
    m_buttonRowLayout = new QHBoxLayout(this);
    m_vLayout = new QVBoxLayout(this);

    m_attributeTable = new GlobalAttributesTableWidget(this);

    m_addBool = new QPushButton("Add Bool", this);
    m_addFloat = new QPushButton("Add Float", this);
    m_addInt = new QPushButton("Add Int", this);
    m_addVector = new QPushButton("Add Vector", this);

    m_buttonRowLayout->addWidget(m_addBool);
    m_buttonRowLayout->addWidget(m_addFloat);
    m_buttonRowLayout->addWidget(m_addInt);
    m_buttonRowLayout->addWidget(m_addVector);

    m_vLayout->addWidget(m_attributeTable);
    m_vLayout->addLayout(m_buttonRowLayout);

    this->setLayout(m_buttonRowLayout);
}

//------------------------------------------------------------------------------
void AttributeDock::connectStuff()
{
    connect(m_addBool, SIGNAL(clicked()), this, SLOT(onAddBoolClicked()));
    connect(m_addFloat, SIGNAL(clicked()),this, SLOT(onAddFloatClicked()));
    connect(m_addInt, SIGNAL(clicked()), this, SLOT(onAddIntClicked()));
    connect(m_addVector, SIGNAL(clicked()), this, SLOT(onAddVector()));
}

//------------------------------------------------------------------------------

