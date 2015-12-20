#include "SceneEditor/GlobalDetailsWidget.h"

#include "SceneEditor/SceneEditorGame.h"

//------------------------------------------------------------------------------
GlobalDetailsWidget::GlobalDetailsWidget(std::shared_ptr<SceneEditorGame> game, QWidget *parent) : QWidget(parent), m_game(game)
{
    m_propertiesLocked = false;

    m_layout =  new QFormLayout(this);

    m_lightPosX = createNumericField("Light Dir. X");
    m_lightPosY = createNumericField("Light Dir. Y");
    m_lightPosZ = createNumericField("Light Dir. Z");

    m_lightColorR = createNumericField("Light Red");
    m_lightColorG = createNumericField("Light Green");
    m_lightColorB = createNumericField("Light Blue");

    this->setLayout(m_layout);

    // register at game
    connect(m_game.get(), SIGNAL(sceneChanged()),
            this, SLOT(sceneChanged()));
}

//------------------------------------------------------------------------------
QDoubleSpinBox *GlobalDetailsWidget::createNumericField(const QString &name)
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
    connect(widget, SIGNAL(valueChanged(double)), this, SLOT(valuesChanged()));
    m_layout->addRow(name, widget);
    return widget;
}

//------------------------------------------------------------------------------
void GlobalDetailsWidget::sceneChanged()
{
    updateUI();
}

//------------------------------------------------------------------------------
void GlobalDetailsWidget::updateUI()
{
    m_propertiesLocked = true;

    auto &lightDirection = m_game->getScene()->getDirectionalLightDirection();
    m_lightPosX->setValue(lightDirection.x());
    m_lightPosY->setValue(lightDirection.y());
    m_lightPosZ->setValue(lightDirection.z());

    auto &lightColor = m_game->getScene()->getLightColor();
    m_lightColorR->setValue(lightColor.x());
    m_lightColorG->setValue(lightColor.y());
    m_lightColorB->setValue(lightColor.z());

    m_propertiesLocked = false;
}

//------------------------------------------------------------------------------
void GlobalDetailsWidget::valuesChanged()
{
    applyScene();
}

//------------------------------------------------------------------------------
void GlobalDetailsWidget::applyScene()
{
    if (m_propertiesLocked)
        return;

    auto &lightDirection = m_game->getScene()->getDirectionalLightDirection();
    lightDirection.setX((float)m_lightPosX->value());
    lightDirection.setY((float)m_lightPosY->value());
    lightDirection.setZ((float)m_lightPosZ->value());

    auto &lightColor = m_game->getScene()->getLightColor();
    lightColor.setX((float)m_lightColorR->value());
    lightColor.setY((float)m_lightColorG->value());
    lightColor.setZ((float)m_lightColorB->value());

    emit m_game->sceneChanged();
}
