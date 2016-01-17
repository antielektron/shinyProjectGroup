#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_DURATION_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_DURATION_H

#include <memory>

#include <QDomElement>

#include "GameLogic/InterpolationCreator/InterpolationCreatorBase.h"
#include "GameLogic/GlobalState.h"
#include "GameLogic/Expressions/ExpressionConstant.h"
#include "GameLogic/Factories/ExpressionFactory.h"
#include "GameLogic/Interpolation/InterpolationLinear.h"
#include "GameLogic/Interpolation/InterpolationForgotName.h"
#include "GameLogic/Interpolation/InterpolationSin.h"
#include "GameLogic/Traits.h"

template <typename Interpolation>
class InterpolationCreatorDuration : public InterpolationCreatorBase
{
public:
    typedef Interpolation InterpolationType;

    InterpolationCreatorDuration();
    InterpolationCreatorDuration(std::unique_ptr<Expression<double>> duration);
    InterpolationCreatorDuration(GlobalState *state, const QDomElement &domElement);
    virtual ~InterpolationCreatorDuration() {}

    virtual std::unique_ptr<InterpolationBase> createInterpolation(double start) override;

    virtual QString type() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

    inline void setDuration(std::unique_ptr<Expression<double>> duration) { m_duration = std::move(duration); }
    inline Expression<double> *getDuration() { return m_duration.get(); }

private:
    std::unique_ptr<Expression<double>> m_duration;
};


//------------------------------------------------------------------------------
template <typename Interpolation>
InterpolationCreatorDuration<Interpolation>::InterpolationCreatorDuration() :
        m_duration(new ExpressionConstant<double>(1))
{}

//------------------------------------------------------------------------------
template <typename Interpolation>
InterpolationCreatorDuration<Interpolation>::InterpolationCreatorDuration(std::unique_ptr<Expression<double>> duration) :
        m_duration(std::move(duration))
{}

//------------------------------------------------------------------------------
template <typename Interpolation>
InterpolationCreatorDuration<Interpolation>::InterpolationCreatorDuration(GlobalState *state, const QDomElement &domElement)
{
    auto duration = domElement.attribute("duration");
    this->m_duration = Factory::createExpressionFromString<double>(state, duration);
}

//------------------------------------------------------------------------------
template <typename Interpolation>
std::unique_ptr<InterpolationBase> InterpolationCreatorDuration<Interpolation>::createInterpolation(double start)
{
    double duration = this->m_duration->evaluate();
    return std::unique_ptr<InterpolationBase>(new Interpolation(start, start + duration));
}

//------------------------------------------------------------------------------
template <typename Interpolation>
QString InterpolationCreatorDuration<Interpolation>::type()
{
    return QString(traits::interpolation_name<InterpolationCreatorDuration<Interpolation>>::value());
}

//------------------------------------------------------------------------------
template <typename Interpolation>
void InterpolationCreatorDuration<Interpolation>::writeToXml(QXmlStreamWriter &writer)
{
    writer.writeStartElement("Interpolation");
    writer.writeAttribute("type", this->type());

    writer.writeAttribute("duration", this->m_duration->string());

    writer.writeEndElement();
}


namespace traits
{
    template <>
    struct interpolation_name<InterpolationCreatorDuration<InterpolationLinear>>
    {
        static const char *value() { return "linear"; }
    };

    template <>
    struct interpolation_name<InterpolationCreatorDuration<InterpolationSin>>
    {
        static const char *value() { return "sinus"; }
    };

    template <>
    struct interpolation_name<InterpolationCreatorDuration<InterpolationForgotName>>
    {
        static const char *value() { return "forgot name"; }
    };
}


#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_DURATION_H
