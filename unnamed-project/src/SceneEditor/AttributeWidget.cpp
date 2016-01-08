#include "SceneEditor/AttributeWidget.h"
#include <QInputDialog>
#include <climits>

AttributeWidget::AttributeWidget(QWidget *parent) :
    QWidget(parent)
{
    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void AttributeWidget::generateWidgets()
{
    m_vLayout = new QVBoxLayout(this);

    m_attributeTable = new GlobalAttributesTableWidget(this);
    m_attributeTable->setMinimumSize(200,100);

    m_addValue = new QPushButton("Add Value", this);
    m_delValue = new QPushButton("Del Value", this);

    m_vLayout->addWidget(m_attributeTable);

    m_vLayout->addWidget(m_addValue);
    m_vLayout->addWidget(m_delValue);

    this->setLayout(m_vLayout);
}

//------------------------------------------------------------------------------
void AttributeWidget::connectStuff()
{
    connect(m_addValue, SIGNAL(clicked()), this, SLOT(onAddValueClicked()));
}

//------------------------------------------------------------------------------
void AttributeWidget::onAddValueClicked()
{
    // determine item type:
    QStringList strList;
    for (const auto typePair : qStringToType)
    {
        strList.append(typePair.first);
    }
    bool ok;
    QString selectedType = QInputDialog::getItem(this,
                                                 "select datatype",
                                                 "datatype",
                                                 strList,
                                                 0,
                                                 false,
                                                 &ok);

    if (!ok)
    {
        return;
    }

    QString keyname = QInputDialog::getText(this,
                                           "enter attribute key",
                                           "key",
                                           QLineEdit::Normal,
                                           "key",
                                           &ok);

    if (!ok)
    {
        return;
    }

    QVariant value;

    switch(qStringToType.at(selectedType))
    {
    case AttributeDatatype::Bool:
    {
        bool val = QInputDialog::getInt(this,
                                        "enter value",
                                        "boolean value",
                                        0,
                                        0,
                                        1,
                                        1,
                                        &ok);

        value = QVariant(val);
        break;
    }
    case AttributeDatatype::Float:
    {
        float val = static_cast<float>(
                    QInputDialog::getDouble(this,
                                            "enter value",
                                            "float value",
                                            0,
                                            -std::numeric_limits<float>::max(),
                                            +std::numeric_limits<float>::max(),
                                            3,
                                            &ok));

        value = QVariant(val);
        break;
    }
    case AttributeDatatype::Int:
    {
        int val = QInputDialog::getInt(this,
                                       "enter value",
                                       "int value",
                                       0,
                                       -std::numeric_limits<int>::max(),
                                       +std::numeric_limits<int>::max(),
                                       1,
                                       &ok);
        value = QVariant(val);
        break;
    }
    case AttributeDatatype::QVector3D:
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

    emit attributeAdded(keyname, value, qStringToType.at(selectedType));
}

//------------------------------------------------------------------------------
void AttributeWidget::onDelValueClicked()
{
    //todo get current selected key and emit deletion signal
}

//------------------------------------------------------------------------------
void AttributeWidget::onAttributesChanged(GlobalState *globalState)
{
    m_attributeTable->onAttributesChanged(globalState);
}

//------------------------------------------------------------------------------
void AttributeWidget::onSingleAttributeAdded(GlobalState *globalState,
                                             const QString &key)
{
    m_attributeTable->onSingleAttributeAdded(globalState, key);
}
