#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_SCENE_ANIMATION_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_SCENE_ANIMATION_ACTION_H

#include <iostream>

#include "GameLogic/Actions/ValueAnimationAction.h"
#include "GameLogic/Traits.h"

template <typename Access>
class SceneAnimationAction : public ValueAnimationAction<Access>
{
public:
    typedef Access AccessType;

    SceneAnimationAction(GlobalState *state);
    SceneAnimationAction(GlobalState *state, const QDomElement &domElement);

    virtual void performAction() override;

    virtual QString string() override;

    virtual QString type() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;
};


//------------------------------------------------------------------------------
template <typename Access>
SceneAnimationAction<Access>::SceneAnimationAction(GlobalState *state) :
        ValueAnimationAction<Access>(state)
{}

//------------------------------------------------------------------------------
template <typename Access>
SceneAnimationAction<Access>::SceneAnimationAction(GlobalState *state, const QDomElement &domElement) :
        ValueAnimationAction<Access>(state, domElement)
{}

//------------------------------------------------------------------------------
template <typename Access>
void SceneAnimationAction<Access>::performAction()
{
    QVector3D dest(this->m_exprX->evaluate(), this->m_exprY->evaluate(), this->m_exprZ->evaluate());

    // TODO find start from scene or so!
    double start = this->m_scene->getGlobalState()->getTime();
    // TODO find interpolation from factory?
    auto interpolation = this->m_interpolationCreator->createInterpolation(start);

    auto animation = std::unique_ptr<ValueAnimation<Access>>(new ValueAnimation<Access>(this->m_scene, std::move(interpolation), Access{this->m_scene}, dest));
    this->m_scene->addAnimation(std::move(animation));
}

//------------------------------------------------------------------------------
template <typename Access>
QString SceneAnimationAction<Access>::string()
{
    // TODO ?
    return this->type();
}

//------------------------------------------------------------------------------
template <typename Access>
QString SceneAnimationAction<Access>::type()
{
    return QString(traits::action_name<SceneAnimationAction<Access>>::value());
}

//------------------------------------------------------------------------------
template <typename Access>
void SceneAnimationAction<Access>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Action");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("x", this->m_exprX->string());
    writer.writeAttribute("y", this->m_exprY->string());
    writer.writeAttribute("z", this->m_exprZ->string());

    this->m_interpolationCreator->writeToXml(writer);

    writer.writeEndElement();
}


namespace traits
{
    template <>
    struct action_name<SceneAnimationAction<AccessLightDirection>>
    {
        static const char *value() { return "animlightdir"; }
    };

    template <>
    struct action_name<SceneAnimationAction<AccessLightColor>>
    {
        static const char *value() { return "animlightcolor"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_SCENE_ANIMATION_ACTION_H
