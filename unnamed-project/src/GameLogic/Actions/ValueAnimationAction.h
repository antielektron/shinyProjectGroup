#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_VALUE_ANIMATION_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_VALUE_ANIMATION_ACTION_H

#include <memory>

#include "GameLogic/Actions/ActionBase.h"
#include "GameLogic/Expressions/ExpressionConstant.h"
#include "GameLogic/InterpolationCreator/InterpolationCreatorDuration.h"
#include "GameLogic/Animations/ValueAnimation.h"
#include "GameLogic/Animations/Access.h"
#include "GameLogic/Traits.h"

#include "GameLogic/Factories/InterpolationCreatorFactory.h"

#include "GameLogic/GlobalState.h"
#include "Scene/Scene.h"

template <typename Access>
class ValueAnimationAction : public ActionBase
{
public:
    typedef Access AccessType;

    ValueAnimationAction(GlobalState *state);
    ValueAnimationAction(GlobalState *state, const QDomElement &domElement);

    inline void setExprX(std::unique_ptr<Expression<double>> expr) { m_exprX = std::move(expr); }
    inline void setExprY(std::unique_ptr<Expression<double>> expr) { m_exprY = std::move(expr); }
    inline void setExprZ(std::unique_ptr<Expression<double>> expr) { m_exprZ = std::move(expr); }

    inline void setInterpolationCreator(std::unique_ptr<InterpolationCreatorBase> interpolationCreator) { m_interpolationCreator = std::move(interpolationCreator); }

    inline Expression<double> *getExprX() { return m_exprX.get(); }
    inline Expression<double> *getExprY() { return m_exprY.get(); }
    inline Expression<double> *getExprZ() { return m_exprZ.get(); }

    inline InterpolationCreatorBase *getInterpolationCreator() { return m_interpolationCreator.get(); }

protected:
    Scene *m_scene;

    std::unique_ptr<Expression<double>> m_exprX;
    std::unique_ptr<Expression<double>> m_exprY;
    std::unique_ptr<Expression<double>> m_exprZ;

    std::unique_ptr<InterpolationCreatorBase> m_interpolationCreator;
};


//------------------------------------------------------------------------------
template <typename Access>
ValueAnimationAction<Access>::ValueAnimationAction(GlobalState *state) :
        m_scene(state->getScene()),
        m_exprX(new ExpressionConstant<double>()),
        m_exprY(new ExpressionConstant<double>()),
        m_exprZ(new ExpressionConstant<double>()),
        m_interpolationCreator(new InterpolationCreatorDuration<InterpolationLinear>())
{}

//------------------------------------------------------------------------------
template <typename Access>
ValueAnimationAction<Access>::ValueAnimationAction(GlobalState *state, const QDomElement &domElement) :
        m_scene(state->getScene())
{
    QString x = domElement.attribute("x");
    m_exprX = Factory::createExpressionFromString<double>(state, x);

    QString y = domElement.attribute("y");
    m_exprY = Factory::createExpressionFromString<double>(state, y);

    QString z = domElement.attribute("z");
    m_exprZ = Factory::createExpressionFromString<double>(state, z);

    for (auto child = domElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "Interpolation")
        {
            m_interpolationCreator = Factory::createInterpolationCreatorFromDomElement(state, child);
            break; // only one interpolation
        }
    }

    if (!m_interpolationCreator)
    {
        // LEGACY!
        QString durationText = domElement.attribute("duration");
        auto duration = Factory::createExpressionFromString<double>(state, durationText);
        m_interpolationCreator.reset(new InterpolationCreatorDuration<InterpolationLinear>(std::move(duration)));
    }
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_VALUE_ANIMATION_ACTION_H
