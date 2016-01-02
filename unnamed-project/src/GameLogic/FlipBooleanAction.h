#ifndef UNNAMED_PROJECT_FLIPBOOLEANACTION_H
#define UNNAMED_PROJECT_FLIPBOOLEANACTION_H

#include "GameLogic/ActionBase.h"

class FlipBooleanAction : public ActionBase
{
public:
    FlipBooleanAction(GlobalState *state,
                      const QString &key);

    virtual ~FlipBooleanAction();

    virtual void performAction() override;

protected:
    QString m_key;
};

#endif // UNNAMED_PROJECT_FLIPBOOLEANACTION_H
