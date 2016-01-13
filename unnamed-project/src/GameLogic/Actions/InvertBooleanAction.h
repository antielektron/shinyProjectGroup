#ifndef UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_BOOLEAN_ACTION_H
#define UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_BOOLEAN_ACTION_H

#include "GameLogic/Actions/ActionBase.h"

class InvertBooleanAction : public ActionBase
{
public:
    InvertBooleanAction(GlobalState *state, const QString &key);
    virtual ~InvertBooleanAction();

    virtual void performAction() override;

    virtual ActionType getActionType() override;

    virtual QString getDataType() override;

    const QString &getKey();

protected:
    QString m_key;
};

#endif // UNNAMED_PROJECT_GAME_LOGIC_ACTIONS_INVERT_BOOLEAN_ACTION_H
