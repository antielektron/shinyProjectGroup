#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include <map>
#include <QString>
#include <QVariant>

class GlobalState
{
public:
    GlobalState();
    virtual ~GlobalState();
    const QVariant &getValue(const QString &key);

protected:
    std::map<QString, QVariant> m_attributes;
};

#endif // GLOBALSTATE_H
