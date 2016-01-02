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
    void setValue(const QString &key,
                  QVariant value);

protected:
    std::map<QString, QVariant> m_attributes;
};

#endif // GLOBALSTATE_H
