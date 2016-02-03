#include "GameLogic/Actions/TriggerAction.h"
#include "GameLogic/GlobalState.h"
#include "Scene/Scene.h"
#include "GameLogic/Expressions/ExpressionConstant.h"
#include "GameLogic/Factories/ExpressionFactory.h"
#include "GameLogic/Animations/TriggerAnimation.h"

//------------------------------------------------------------------------------
TriggerAction::TriggerAction(GlobalState *state) :
        m_scene(state->getScene()),
        m_delay(new ExpressionConstant<double>(0))
{}

//------------------------------------------------------------------------------
TriggerAction::TriggerAction(GlobalState *state, const QDomElement &domElement) :
        m_scene(state->getScene())
{
    m_name = domElement.attribute("name");

    auto delay = domElement.attribute("delay");
    m_delay = Factory::createExpressionFromString<double>(state, delay);
}

//------------------------------------------------------------------------------
void TriggerAction::performAction()
{
    // TODO find start from scene or so!
    double time = m_scene->getGlobalState()->getTime() + m_delay->evaluate();

    auto animation = std::unique_ptr<TriggerAnimation>(new TriggerAnimation(this->m_scene, m_name, time));
    m_scene->addAnimation(std::move(animation));
}

//------------------------------------------------------------------------------
QString TriggerAction::string()
{
    return this->type() + " " + m_name;
}

//------------------------------------------------------------------------------
QString TriggerAction::type()
{
    return QString(traits::action_name<TriggerAction>::value());
}

//------------------------------------------------------------------------------
void TriggerAction::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Action");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("name", m_name);
    writer.writeAttribute("delay", m_delay->string());

    writer.writeEndElement();
}

//------------------------------------------------------------------------------
void TriggerAction::setDelay(std::unique_ptr<Expression<double>> delay)
{
    m_delay = std::move(delay);
}

//------------------------------------------------------------------------------
Expression<double> *TriggerAction::getDelay()
{
    return m_delay.get();
}

//------------------------------------------------------------------------------
void TriggerAction::setName(const QString &name)
{
    m_name = name;
}

//------------------------------------------------------------------------------
const QString &TriggerAction::getName()
{
    return m_name;
}
