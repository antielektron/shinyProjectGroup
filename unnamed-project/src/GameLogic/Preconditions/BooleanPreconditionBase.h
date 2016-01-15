#ifndef UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H
#define UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H

#include <memory>
#include <vector>
#include <QDomElement>

#include "GameLogic/Preconditions/PreconditionBase.h"

class BooleanPreconditionBase : public PreconditionBase
{
public:
    BooleanPreconditionBase();
    BooleanPreconditionBase(const QDomElement &domElement);
    virtual ~BooleanPreconditionBase() {}

    void addCondition(std::unique_ptr<PreconditionBase> condition);
    void removeCondition(size_t i);

    // TODO begin/end

    virtual QString string() override;

    virtual void writeToXml(QXmlStreamWriter &writer) override;

protected:
    std::vector<std::unique_ptr<PreconditionBase>> m_conditions;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_PRECONDITIONS_BOOLEAN_PRECONDITION_BASE_H
