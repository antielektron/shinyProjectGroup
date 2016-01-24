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

    m_playerPosX = createNumericField("Player Pos. X");
    m_playerPosY = createNumericField("Player Pos. Y");
    m_playerPosZ = createNumericField("Player Pos. Z");

    m_playerRotX = createNumericField("Player Rot. X");
    m_playerRotY = createNumericField("Player Rot. Y");
    m_playerRotZ = createNumericField("Player Rot. Z");

    this->setLayout(m_layout);

    // register at game (done in Window)
    connect(m_game.get(), SIGNAL(sceneChanged()), this, SLOT(sceneChanged()));
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

    auto &playerPosition = m_game->getScene()->getPlayerPosition();
    m_playerPosX->setValue(playerPosition.x());
    m_playerPosY->setValue(playerPosition.y());
    m_playerPosZ->setValue(playerPosition.z());

    auto &playerRotation = m_game->getScene()->getPlayerRotation();
    m_playerRotX->setValue(playerRotation.x());
    m_playerRotY->setValue(playerRotation.y());
    m_playerRotZ->setValue(playerRotation.z());

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

    auto &playerPosition = m_game->getScene()->getPlayerPosition();
    playerPosition.setX((float)m_playerPosX->value());
    playerPosition.setY((float)m_playerPosY->value());
    playerPosition.setZ((float)m_playerPosZ->value());

    auto &playerRotation = m_game->getScene()->getPlayerRotation();
    playerRotation.setX((float)m_playerRotX->value());
    playerRotation.setY((float)m_playerRotY->value());
    playerRotation.setZ((float)m_playerRotZ->value());

    emit m_game->sceneChanged();
}
