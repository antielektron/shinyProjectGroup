#include "SceneEditor/ObjectDetailsWidget.h"

#include "SceneEditor/SceneEditorGame.h"
#include "Scene/Object.h"
#include "Scene/ObjectGroup.h"

ObjectDetailsWidget::ObjectDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    m_layout =  new QFormLayout(this);

    m_modelSelection = new QComboBox(this);
    m_layout->addRow("Model", m_modelSelection);

    m_posX = createNumericField("X");
    m_posY = createNumericField("Y");
    m_posZ = createNumericField("Z");

    m_rotYaw = createNumericField("Yaw");
    m_rotPitch = createNumericField("Pitch");
    m_rotRoll = createNumericField("Roll");

    m_applyButton = new QPushButton("Apply", this);
    m_layout->addWidget(m_applyButton);

    // Scaling?

    // Material?

    this->setLayout(m_layout);

    connect(m_applyButton, SIGNAL(clicked()), this, SLOT(applyClicked()));
    connect(m_game.get(), SIGNAL(currentObjectChanged()), this, SLOT(onCurrentObjectChanged()));
    connect(m_game.get(), SIGNAL(modelsChanged()), this, SLOT(onModelsChanged()));

    // Get current object from game if there is already any.
    onCurrentObjectChanged();
}

QLineEdit *ObjectDetailsWidget::createNumericField(const QString &name)
{
    QLineEdit *widget = new QLineEdit(this);
    m_layout->addRow(name, widget);
    return widget;
}

void ObjectDetailsWidget::onModelsChanged()
{
    m_models.clear();
    m_game->getModels(m_models);
}

void ObjectDetailsWidget::onCurrentObjectChanged()
{
    m_currentObject = m_game->getCurrentObject();
    m_currentObjectGroup = m_game->getCurrentObjectGroup();

    if (m_currentObject != nullptr)
    {
        updateCurrentObject(m_currentObject);
    }
    else if (m_currentObjectGroup != nullptr)
    {
        updateCurrentObjectGroup(m_currentObjectGroup);
    }
    else
    {
        // TODO set empty
    }
}

void ObjectDetailsWidget::applyClicked()
{
    if (m_currentObject != nullptr)
    {
        applyCurrentObject(m_currentObject);
    }
    else if (m_currentObjectGroup != nullptr)
    {
        applyCurrentObjectGroup(m_currentObjectGroup);
    }

    m_game->currentObjectModified();
}

void ObjectDetailsWidget::updateCurrentObject(Object *object)
{
    m_modelSelection->setEnabled(true);

    int index = m_modelSelection->findData(qVariantFromValue((void *)object->getModel()));
    m_modelSelection->setCurrentIndex(index);

    updateCurrentObjectBase(object);
}

void ObjectDetailsWidget::updateCurrentObjectGroup(ObjectGroup *objectGroup)
{
    m_modelSelection->setEnabled(false);
    updateCurrentObjectBase(objectGroup);
}

void ObjectDetailsWidget::updateCurrentObjectBase(ObjectBase *object)
{
    m_posX->setText(QString::number(object->getPosition().x()));
    m_posY->setText(QString::number(object->getPosition().y()));
    m_posZ->setText(QString::number(object->getPosition().z()));

    m_rotYaw->setText(QString::number(object->getRotation().y()));
    m_rotPitch->setText(QString::number(object->getRotation().x()));
    m_rotRoll->setText(QString::number(object->getRotation().z()));
}

void ObjectDetailsWidget::applyCurrentObject(Object *object)
{
    // TODO
    applyCurrentObjectBase(object);
}

void ObjectDetailsWidget::applyCurrentObjectGroup(ObjectGroup *objectGroup)
{
    applyCurrentObjectBase(objectGroup);
}

void ObjectDetailsWidget::applyCurrentObjectBase(ObjectBase *object)
{
    object->getPosition().setX(m_posX->text().toFloat());
    object->getPosition().setY(m_posY->text().toFloat());
    object->getPosition().setZ(m_posZ->text().toFloat());

    object->getRotation().setY(m_rotYaw->text().toFloat());
    object->getRotation().setX(m_rotPitch->text().toFloat());
    object->getRotation().setZ(m_rotRoll->text().toFloat());
}
