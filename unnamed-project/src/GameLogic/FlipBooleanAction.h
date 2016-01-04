#ifndef UNNAMED_PROJECT_FLIPBOOLEANACTION_H
#define UNNAMED_PROJECT_FLIPBOOLEANACTION_H

#include <QString>
#include "GameLogic/ActionBase.h"

class FlipBooleanAction : public ActionBase
{
public:
    FlipBooleanAction(GlobalState *state,
                      const QString &key);

    virtual ~FlipBooleanAction();

    virtual void performAction() override;

    virtual ActionType getActionType() override;

    virtual QString getDataType() override;

    const QString &getKey();

protected:
    QString m_key;
};

#endif // UNNAMED_PROJECT_FLIPBOOLEANACTION_H
