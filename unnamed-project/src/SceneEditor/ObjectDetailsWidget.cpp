#include "SceneEditor/ObjectDetailsWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

//DEBUG
#include <iostream>

//------------------------------------------------------------------------------
ObjectDetailsWidget::ObjectDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    m_objectPropertiesLocked = false;

    m_layout =  new QFormLayout(this);

    m_modelSelection = new QComboBox(this);
    m_layout->addRow("Model", m_modelSelection);

    m_posX = createNumericField("X");
    m_posY = createNumericField("Y");
    m_posZ = createNumericField("Z");

    m_rotYaw = createNumericField("Yaw");
    m_rotPitch = createNumericField("Pitch");
    m_rotRoll = createNumericField("Roll");

    m_scaleX = createNumericField("scale x");
    m_scaleY = createNumericField("scale y");
    m_scaleZ = createNumericField("scale z");

    connect(m_modelSelection, SIGNAL(currentTextChanged(QString)),
            this, SLOT(applyValues()));

    // Material?

    this->setLayout(m_layout);

    m_currentObject = nullptr;
}

//------------------------------------------------------------------------------
QDoubleSpinBox *ObjectDetailsWidget::createNumericField(const QString &name)
{
    QDoubleSpinBox *widget = new QDoubleSpinBox(this);
    // set limits for widget
    // (using std::numeric_limits is a bad idea here,
    // because the spin box will be sized so that
    // every valid value fits (and may fills the whole
    // parent widget)
    widget->setMaximum(10e10);
    widget->setMinimum(-10e10);

    widget->setDecimals(3);
    widget->setSingleStep(0.1);

    // connect with
    connect(widget, SIGNAL(valueChanged(double)), this, SLOT(applyValues()));
    m_layout->addRow(name, widget);
    return widget;
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::modelsChanged()
{
    fillModelSelection();
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::currentObjectChanged(ObjectBase *object)
{
    // New current object from game
    m_currentObject = object;

    // Update widgets:
    switch (object->getObjectType())
    {
    case ObjectType::Object:
    {
        updateCurrentObject(static_cast<Object *>(object));
        break;
    }
    case ObjectType::ObjectGroup:
    {
        updateCurrentObjectGroup(static_cast<ObjectGroup *>(object));
        break;
    }
    default:
    {
        // Should not happen, just in case..
        updateCurrentObjectBase(object);
    }
    }
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyValues()
{
    if (!m_currentObject)
    {
        return;
    }

    switch(m_currentObject->getObjectType())
    {
    case ObjectType::Object:
    {
        applyCurrentObject(static_cast<Object *>(m_currentObject));
        break;
    }
    case ObjectType::ObjectGroup:
    {
        applyCurrentObjectGroup(static_cast<ObjectGroup *>(m_currentObject));
        break;
    }
    default:
    {
        // Should not happen, just in case..
        applyCurrentObjectBase(m_currentObject);
    }
    }

    m_game->currentObjectModified(m_currentObject);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObject(Object *object)
{
    m_objectPropertiesLocked = true;
    m_modelSelection->setEnabled(true);

    int index = m_modelSelection->findText(
                QString::fromStdString(object->getModel()->getName()));
    m_modelSelection->setCurrentIndex(index);

    updateCurrentObjectBase(object);
    m_objectPropertiesLocked = false;
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObjectGroup(ObjectGroup *objectGroup)
{
    m_modelSelection->setEnabled(false);
    updateCurrentObjectBase(objectGroup);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObjectBase(ObjectBase *object)
{
    m_posX->setValue(object->getPosition().x());
    m_posY->setValue(object->getPosition().y());
    m_posZ->setValue(object->getPosition().z());

    m_rotYaw->setValue(object->getRotation().y());
    m_rotPitch->setValue(object->getRotation().x());
    m_rotRoll->setValue(object->getRotation().z());

    m_scaleX->setValue(object->getScaling().x());
    m_scaleY->setValue(object->getScaling().y());
    m_scaleZ->setValue(object->getScaling().z());
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyCurrentObject(Object *object)
{
    object->setModel(m_game->getScene()->getModel(
                         m_modelSelection->currentText().toStdString()));
    applyCurrentObjectBase(object);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyCurrentObjectGroup(ObjectGroup *objectGroup)
{
    applyCurrentObjectBase(objectGroup);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyCurrentObjectBase(ObjectBase *object)
{
    if (m_objectPropertiesLocked)
    {
        return;
    }
    object->getPosition().setX((float)m_posX->value());
    object->getPosition().setY((float)m_posY->value());
    object->getPosition().setZ((float)m_posZ->value());

    object->getRotation().setY((float)m_rotYaw->value());
    object->getRotation().setX((float)m_rotPitch->value());
    object->getRotation().setZ((float)m_rotRoll->value());

    object->getScaling().setX((float)m_scaleX->value());
    object->getScaling().setY((float)m_scaleY->value());
    object->getScaling().setZ((float)m_scaleZ->value());

    object->updateWorld();
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::fillModelSelection()
{
    m_modelSelection->clear();
    auto models = m_game->getScene()->getModels();
    std::cout << "update models" << std::endl;
    for (auto &model : models)
    {
        m_modelSelection->addItem(QString::fromStdString( model.second->getName()));
    }
}
