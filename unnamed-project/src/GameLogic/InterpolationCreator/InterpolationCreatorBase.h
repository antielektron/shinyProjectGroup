#ifndef UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_BASE_H

#include <QString>
#include <QXmlStreamWriter>

class InterpolationBase;

class InterpolationCreatorBase
{
public:
    InterpolationCreatorBase() = default;
    virtual ~InterpolationCreatorBase() = default;

    virtual std::unique_ptr<InterpolationBase> createInterpolation(double start) = 0;

    virtual QString type() = 0;

    virtual void writeToXml(QXmlStreamWriter &writer) = 0;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_INTERPOLATION_CREATOR_INTERPOLATION_CREATOR_BASE_H
