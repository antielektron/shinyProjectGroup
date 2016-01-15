#include "SceneEditor/AttributesWidget.h"
#include "SceneEditor/SceneEditorGame.h"

#include <QInputDialog>
#include <QBoxLayout>

//DEBUG
#include <iostream>

//------------------------------------------------------------------------------
AttributesWidget::AttributesWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) :
        QWidget(parent),
        m_game(game)
{
    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void AttributesWidget::generateWidgets()
{
    this->setLayout(new QVBoxLayout());

    m_attributeTable = new AttributesTableWidget(m_game, this);
    m_attributeTable->setMinimumSize(200,100);
    this->layout()->addWidget(m_attributeTable);

    QWidget *container = new QWidget(this);
    container->setLayout(new QHBoxLayout());
    container->layout()->setMargin(0);
    this->layout()->addWidget(container);

    m_addValue = new QPushButton("Add Value", container);
    container->layout()->addWidget(m_addValue);

    m_deleteValue = new QPushButton("Delete Value", container);
    container->layout()->addWidget(m_deleteValue);
}

//------------------------------------------------------------------------------
void AttributesWidget::connectStuff()
{
    connect(m_addValue, SIGNAL(clicked()), this, SLOT(onAddValueClicked()));
    connect(m_deleteValue, SIGNAL(clicked()), this, SLOT(onDeleteValueClicked()));

    // connect to game!
    connect(m_game.get(), SIGNAL(attributesChanged()), this, SLOT(onAttributesChanged()));
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(onAttributesChanged()));
}

//------------------------------------------------------------------------------
void AttributesWidget::onAddValueClicked()
{
    bool ok;

    // determine item type
    QStringList typeList;

    typeList.push_back(QMetaType::typeName(QMetaType::Bool));
    typeList.push_back(QMetaType::typeName(QMetaType::Int));
    typeList.push_back(QMetaType::typeName(QMetaType::Float));
    typeList.push_back(QMetaType::typeName(QMetaType::QVector3D));

    QString typeName = QInputDialog::getItem(this, "select type", "type", typeList, 0, false, &ok);

    if (!ok)
    {
        return;
    }

    QString key = QInputDialog::getText(this, "enter attribute key", "key", QLineEdit::Normal, "empty", &ok);

    if (!ok)
    {
        return;
    }

    QVariant value;

    switch (QMetaType::type(typeName.toLatin1().data()))
    {
        case QMetaType::Bool:
        {
            bool val = QInputDialog::getInt(this,
                                            "enter bool value",
                                            "value",
                                            0,
                                            0,
                                            1,
                                            1,
                                            &ok) != 0;

            value = QVariant(val);
            break;
        }
        case QMetaType::Float:
        {
            float val = static_cast<float>(
                        QInputDialog::getDouble(this,
                                                "enter float value",
                                                "value",
                                                0,
                                                -std::numeric_limits<float>::max(),
                                                +std::numeric_limits<float>::max(),
                                                3,
                                                &ok));

            value = QVariant(val);
            break;
        }
        case QMetaType::Int:
        {
            int val = QInputDialog::getInt(this,
                                           "enter int value",
                                           "value",
                                           0,
                                           -std::numeric_limits<int>::max(),
                                           +std::numeric_limits<int>::max(),
                                           1,
                                           &ok);

            value = QVariant(val);
            break;
        }
        case QMetaType::QVector3D:
        {
            QVector3D val;
            QString coord[] = {"x","y","z"};
            for (int i = 0; i < 3; i++)
            {
                val[i] = static_cast<float>(
                            QInputDialog::getDouble(this,
                                                    "enter vector coordinates",
                                                    coord[i],
                                                    0,
                                                    -std::numeric_limits<float>::max(),
                                                    +std::numeric_limits<float>::max(),
                                                    3,
                                                    &ok));
                if (!ok)
                {
                    return;
                }
            }
            value = QVariant(val);
            break;
        }
    }

    if (!ok)
    {
        return;
    }

    // now we have gathered all the data...
    m_game->addAttribute(key, value);
}

//------------------------------------------------------------------------------
void AttributesWidget::onDeleteValueClicked()
{
    QModelIndexList selectedList = m_attributeTable->selectionModel()->selectedRows();
    for (int i = 0; i < selectedList.size(); i++)
    {
        m_game->deleteAttribute(selectedList.at(i).data(0).toString());
    }
}
