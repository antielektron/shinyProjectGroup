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
    m_currentObject = nullptr;

    generateWidgets();
    connectStuff();
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::generateWidgets()
{
    m_layout =  new QFormLayout(this);
    this->setLayout(m_layout);

    m_name = new QLineEdit(this);
    m_layout->addRow("Name", m_name);

    m_modelSelection = new QComboBox(this);
    m_layout->addRow("Model", m_modelSelection);

    m_interactionEvent = new QLineEdit(this);
    m_layout->addRow("Interaction Event", m_interactionEvent);

    m_posX = createNumericField("X");
    m_posY = createNumericField("Y");
    m_posZ = createNumericField("Z");

    m_rotYaw = createNumericField("Yaw");
    m_rotPitch = createNumericField("Pitch");
    m_rotRoll = createNumericField("Roll");

    m_scaleX = createNumericField("Scale X");
    m_scaleY = createNumericField("Scale Y");
    m_scaleZ = createNumericField("Scale Z");

    m_ambientRed = createNumericField("Ambient Red");
    m_ambientGreen = createNumericField("Ambient Green");
    m_ambientBlue = createNumericField("Ambient Blue");

    m_diffuseRed = createNumericField("Diffuse Red");
    m_diffuseGreen = createNumericField("Diffuse Green");
    m_diffuseBlue = createNumericField("Diffuse Blue");

    m_specularRed = createNumericField("Specular Red");
    m_specularGreen = createNumericField("Specular Green");
    m_specularBlue = createNumericField("Specular Blue");
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::connectStuff()
{
    connect(m_name, SIGNAL(editingFinished()), this, SLOT(applyValues()));
    connect(m_modelSelection, SIGNAL(currentTextChanged(QString)), this, SLOT(applyValues()));
    connect(m_interactionEvent, SIGNAL(editingFinished()), this, SLOT(applyValues()));

    // connect to game autonomiously!
    connect(m_game.get(), SIGNAL(modelsChanged()), this, SLOT(modelsChanged()));
    connect(m_game.get(), SIGNAL(currentObjectChanged()), this, SLOT(currentObjectChanged()));
    connect(m_game.get(), SIGNAL(objectsChanged()), this, SLOT(currentObjectChanged()));

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
void ObjectDetailsWidget::currentObjectChanged()
{
    // New current object from game
    m_currentObject = m_game->getCurrentObject();

    if (m_currentObject == nullptr)
    {
        // TODO disable UI
        return;
    }

    m_objectPropertiesLocked = true;

    // Update widgets:
    switch (m_currentObject->getObjectType())
    {
    case ObjectType::Object:
    {
        updateCurrentObject(static_cast<Object *>(m_currentObject));
        break;
    }
    case ObjectType::ObjectGroup:
    {
        updateCurrentObjectGroup(static_cast<ObjectGroup *>(m_currentObject));
        break;
    }
    default:
    {
        // Should not happen, just in case..
        updateCurrentObjectBase(m_currentObject);
    }
    }

    m_objectPropertiesLocked = false;
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyValues()
{
    if (!m_currentObject || m_objectPropertiesLocked)
        return;

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

    m_game->notifyCurrentObjectChanged(m_currentObject);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObject(Object *object)
{
    m_modelSelection->setEnabled(true);

    int index = m_modelSelection->findText(
                QString::fromStdString(object->getModel()->getName()));
    m_modelSelection->setCurrentIndex(index);

    m_interactionEvent->setEnabled(true);
    m_interactionEvent->setText(object->getInteractionEvent());

    m_ambientRed->setEnabled(true);
    m_ambientGreen->setEnabled(true);
    m_ambientBlue->setEnabled(true);

    m_diffuseRed->setEnabled(true);
    m_diffuseGreen->setEnabled(true);
    m_diffuseBlue->setEnabled(true);

    m_specularRed->setEnabled(true);
    m_specularGreen->setEnabled(true);
    m_specularBlue->setEnabled(true);

    m_ambientRed->setValue(object->getAmbientColor().x());
    m_ambientGreen->setValue(object->getAmbientColor().y());
    m_ambientBlue->setValue(object->getAmbientColor().z());

    m_diffuseRed->setValue(object->getDiffuseColor().x());
    m_diffuseGreen->setValue(object->getDiffuseColor().y());
    m_diffuseBlue->setValue(object->getDiffuseColor().z());

    m_specularRed->setValue(object->getSpecularColor().x());
    m_specularGreen->setValue(object->getSpecularColor().y());
    m_specularBlue->setValue(object->getSpecularColor().z());

    updateCurrentObjectBase(object);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObjectGroup(ObjectGroup *objectGroup)
{
    m_modelSelection->setEnabled(false);

    m_interactionEvent->setEnabled(false);

    m_ambientRed->setEnabled(false);
    m_ambientGreen->setEnabled(false);
    m_ambientBlue->setEnabled(false);

    m_diffuseRed->setEnabled(false);
    m_diffuseGreen->setEnabled(false);
    m_diffuseBlue->setEnabled(false);

    m_specularRed->setEnabled(false);
    m_specularGreen->setEnabled(false);
    m_specularBlue->setEnabled(false);


    updateCurrentObjectBase(objectGroup);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::updateCurrentObjectBase(ObjectBase *object)
{
    m_name->setText(object->getName());

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
    if (m_objectPropertiesLocked)
        return;

    object->setModel(m_game->getScene()->getModel(m_modelSelection->currentText().toStdString()));

    object->setInteractionEvent(m_interactionEvent->text());

    object->getAmbientColor().setX((float)m_ambientRed->value());
    object->getAmbientColor().setY((float)m_ambientGreen->value());
    object->getAmbientColor().setZ((float)m_ambientBlue->value());

    object->getDiffuseColor().setX((float)m_diffuseRed->value());
    object->getDiffuseColor().setY((float)m_diffuseGreen->value());
    object->getDiffuseColor().setZ((float)m_diffuseBlue->value());

    object->getSpecularColor().setX((float)m_specularRed->value());
    object->getSpecularColor().setY((float)m_specularGreen->value());
    object->getSpecularColor().setZ((float)m_specularBlue->value());

    applyCurrentObjectBase(object);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyCurrentObjectGroup(ObjectGroup *objectGroup)
{
    if (m_objectPropertiesLocked)
        return;

    applyCurrentObjectBase(objectGroup);
}

//------------------------------------------------------------------------------
void ObjectDetailsWidget::applyCurrentObjectBase(ObjectBase *object)
{
    if (m_objectPropertiesLocked)
        return;

    object->setName(m_name->text());

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
    m_objectPropertiesLocked = true;

    m_modelSelection->clear();
    auto models = m_game->getScene()->getModels();
    std::cout << "update models" << std::endl;
    for (auto &model : models)
    {
        m_modelSelection->addItem(QString::fromStdString( model.second->getName()));
    }

    currentObjectChanged();

    m_objectPropertiesLocked = false;
}
