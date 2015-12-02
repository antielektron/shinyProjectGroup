#include "SceneEditor/ObjectDetailsWidget.h"

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

    // Scaling?

    // Material?

    this->setLayout(m_layout);
}

QLineEdit *ObjectDetailsWidget::createNumericField(const QString &name)
{
    QLineEdit *widget = new QLineEdit(this);
    m_layout->addRow(name, widget);
    return widget;
}
