#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_OBJECT_ANIMATION_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_OBJECT_ANIMATION_ACTION_H

#include <iostream>

#include "GameLogic/Actions/ValueAnimationAction.h"
#include "GameLogic/Animations/ObjectAnimation.h"
#include "GameLogic/Traits.h"

#include "GameLogic/Interpolation/InterpolationLinear.h"

template <typename Access>
class ObjectAnimationAction : public ValueAnimationAction<Access>
{
public:
    ObjectAnimationAction(GlobalState *state);
    ObjectAnimationAction(GlobalState *state, const QDomElement &domElement);

    virtual void performAction() override;

    virtual QString string() override;

    virtual QString type() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline void setObjectName(const QString &name) { m_objectName = name; }

    inline const QString &getObjectName() { return m_objectName; }

protected:
    QString m_objectName;
};


//------------------------------------------------------------------------------
template <typename Access>
ObjectAnimationAction<Access>::ObjectAnimationAction(GlobalState *state) :
        ValueAnimationAction<Access>(state),
        m_objectName("empty")
{}

//------------------------------------------------------------------------------
template <typename Access>
ObjectAnimationAction<Access>::ObjectAnimationAction(GlobalState *state, const QDomElement &domElement) :
        ValueAnimationAction<Access>(state, domElement)
{
    m_objectName = domElement.attribute("object");
}

//------------------------------------------------------------------------------
template <typename Access>
void ObjectAnimationAction<Access>::performAction()
{
    QVector3D dest(this->m_exprX->evaluate(), this->m_exprY->evaluate(), this->m_exprZ->evaluate());

    auto duration = this->m_duration->evaluate();
    // TODO find start from scene or so!
    double start = this->m_scene->getGlobalState()->getTime();
    // TODO find interpolation from factory?
    auto interpolation = std::unique_ptr<InterpolationBase>(new InterpolationLinear(start, start+duration));

    auto object = static_cast<typename Access::ObjectType *>(this->m_scene->findObjectByName(this->m_scene->getSceneRoot(), this->m_objectName));
    if (!object)
    {
        std::cerr << "Did not found object '" << this->m_objectName.toStdString() << "'" << std::endl;
    }

    auto animation = std::unique_ptr<ObjectAnimation<Access>>(new ObjectAnimation<Access>(object, this->m_scene, std::move(interpolation), Access{object}, dest));
    this->m_scene->addAnimation(std::move(animation));
}

//------------------------------------------------------------------------------
template <typename Access>
QString ObjectAnimationAction<Access>::string()
{
    // TODO ?
    return this->type();
}

//------------------------------------------------------------------------------
template <typename Access>
QString ObjectAnimationAction<Access>::type()
{
    return QString(traits::action_name<ObjectAnimationAction<Access>>::value());
}

//------------------------------------------------------------------------------
template <typename Access>
void ObjectAnimationAction<Access>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Action");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("x", this->m_exprX->string());
    writer.writeAttribute("y", this->m_exprY->string());
    writer.writeAttribute("z", this->m_exprZ->string());

    writer.writeAttribute("duration", this->m_duration->string());

    writer.writeAttribute("object", this->m_objectName);

    writer.writeEndElement();
}


namespace traits
{
    template <>
    struct action_name<ObjectAnimationAction<AccessObjectPosition>>
    {
        static const char *value() { return "animpos"; }
    };

    template <>
    struct action_name<ObjectAnimationAction<AccessObjectRotation>>
    {
        static const char *value() { return "animrot"; }
    };

    template <>
    struct action_name<ObjectAnimationAction<AccessObjectAmbientColor>>
    {
        static const char *value() { return "animambientcolor"; }
    };

    template <>
    struct action_name<ObjectAnimationAction<AccessObjectDiffuseColor>>
    {
        static const char *value() { return "animdiffusecolor"; }
    };

    template <>
    struct action_name<ObjectAnimationAction<AccessObjectSpecularColor>>
    {
        static const char *value() { return "animspecularcolor"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_OBJECT_ANIMATION_ACTION_H
