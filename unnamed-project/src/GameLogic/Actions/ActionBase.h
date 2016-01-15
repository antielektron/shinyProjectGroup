#ifndef UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H
#define UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H

#include <QString>
#include <QXmlStreamWriter>

class ActionBase
{
public:
    ActionBase() {}
    virtual ~ActionBase() {}

    virtual void performAction() = 0;

    virtual QString string() = 0;

    virtual QString type() = 0;

    virtual void writeToXml(QXmlStreamWriter &writer) = 0;
};

#endif // UNNAMED_PROJET_GAME_LOGIC_ACTIONS_ACTION_BASE_H
