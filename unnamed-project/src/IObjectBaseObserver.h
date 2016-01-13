#ifndef UNNAMED_PROJECT_IOBJECTBASEOBSERVER_H
#define UNNAMED_PROJECT_IOBJECTBASEOBSERVER_H

class ObjectBase;

class IObjectBaseObserver
{
public:
    IObjectBaseObserver() = default;
    virtual ~IObjectBaseObserver() = default;

    virtual void notify(ObjectBase *obj) = 0;
};

#endif // UNNAMED_PROJECT_IOBJECTBASEOBSERVER_H
