#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H

#include <QString>
#include <QXmlStreamWriter>

class PreconditionBase
{
public:
    PreconditionBase() {};
    virtual ~PreconditionBase() {};

    virtual bool evaluateCondition() = 0;

    virtual QString string() = 0;

    virtual QString type() = 0;

    virtual void writeToXml(QXmlStreamWriter &writer) = 0;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_PRECONDITION_BASE_H
